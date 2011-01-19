
(define (make-vect x y)
    (cons x y))

(define (xcor-vect vec)
    (car vec))

(define (ycor-vect vec)
    (cdr vec))

(define (add-vect v1 v2)
    (make-vect (+ (xcor-vect v1) (xcor-vect v2))
                 (+ (ycor-vect v1) (ycor-vect v2))))

(define (sub-vect v1 v2)
    (make-vect (- (xcor-vect v1) (xcor-vect v2))
                 (- (ycor-vect v1) (ycor-vect v2))))

(define (scale-vect s v)
    (make-vect (* s (xcor-vect v))
                 (* s (ycor-vect v))))

; --- SVG specifics ---

(define (svg-start)
    (display "<svg xmlns:svg='http://www.w3.org/2000/svg' xmlns='http://www.w3.org/2000/svg' width='100' height='100'>")
    (newline))

(define (svg-draw-line v1 v2)
    (display "  <path d='M ")
    (display (* 100 (xcor-vect v1)))
    (display " ")
    (display (* 100 (- 1 (ycor-vect v1))))
    (display " ")
    (display (* 100 (xcor-vect v2)))
    (display " ")
    (display (* 100 (- 1 (ycor-vect v2))))
    (display "' style='stroke:#000000;'/>")
    (newline))

(define (svg-end)
    (display "</svg>")
    (newline)
    )

; --- Painter code ---

(define (make-segment vector1 vector2)
    (cons vector1 vector2))

(define (start-segment segment)
    (car segment))

(define (end-segment segment)
    (cdr segment))

(define (frame-coord-map frame)
    (lambda (v)
            (add-vect (origin-frame frame)
                      (add-vect (scale-vect (xcor-vect v)
                                            (edge1-frame frame))
                                (scale-vect (ycor-vect v)
                                            (edge2-frame frame))))))

(define (make-frame origin edge1 edge2)
    (list origin edge1 edge2))

(define (origin-frame frame)
    (car frame))

(define (edge1-frame frame)
    (cadr frame))

(define (edge2-frame frame)
    (caddr frame))

(define (segments->painter segment-list)
    (lambda (frame)
            (for-each
                (lambda (segment)
                        (draw-line
                            ((frame-coord-map frame) (start-segment segment))
                            ((frame-coord-map frame) (end-segment segment))))
                      segment-list)))

(define (transform-painter painter origin corner1 corner2)
    (lambda (frame)
        (let ((m (frame-coord-map frame)))
            (let ((new-origin (m origin)))
                (painter
                    (make-frame new-origin
                                (sub-vect (m corner1) new-origin)
                                (sub-vect (m corner2) new-origin)))))))

; --- Transforms ---

(define (flip-vert painter)
        (transform-painter painter
                           (make-vect 0.0 1.0)   ; new origin
                           (make-vect 1.0 1.0)   ; new end of edge1
                           (make-vect 0.0 0.0))) ; new end of edge2

(define (flip-horiz painter)
        (transform-painter painter
                           (make-vect 1.0 0.0)   ; new origin
                           (make-vect 0.0 0.0)   ; new end of edge1
                           (make-vect 1.0 1.0))) ; new end of edge2

(define (shrink-to-upper-right painter)
        (transform-painter painter
                           (make-vect 0.5 0.5)
                           (make-vect 1.0 0.5)
                           (make-vect 0.5 1.0)))

(define (rotate90 painter)
        (transform-painter painter
                           (make-vect 1.0 0.0)
                           (make-vect 1.0 1.0)
                           (make-vect 0.0 0.0)))

(define (squash-inwards painter)
        (transform-painter painter
                           (make-vect 0.0 0.0)
                           (make-vect 0.65 0.35)
                           (make-vect 0.35 0.65)))

(define (beside painter1 painter2)
        (let ((split-point (make-vect 0.5 0.0)))
             (let ((paint-left
                    (transform-painter painter1
                                       (make-vect 0.0 0.0)
                                       split-point
                                       (make-vect 0.0 1.0)))
                   (paint-right
                    (transform-painter painter2
                                       split-point
                                       (make-vect 1.0 0.0)
                                       (make-vect 0.5 1.0))))
               (lambda (frame)
                 (paint-left frame)
                 (paint-right frame)))))


(define (below painter1 painter2)
        (let ((split-point (make-vect 0.0 0.5)))
             (let ((paint-left
                    (transform-painter painter1
                                       (make-vect 0.0 0.0)
                                       (make-vect 1.0 0.0)
                                       split-point))
                   (paint-right
                    (transform-painter painter2
                                       split-point
                                       (make-vect 1.0 0.5)
                                       (make-vect 0.0 1.0))))
               (lambda (frame)
                 (paint-left frame)
                 (paint-right frame)))))

(define (right-split painter n)
        (if (= n 0)
            painter
            (let ((smaller (right-split painter (- n 1))))
                 (beside painter (below smaller smaller)))))

(define (up-split painter n)
        (if (= n 0)
            painter
            (let ((smaller (up-split painter (- n 1))))
                 (below painter (beside smaller smaller)))))

(define (corner-split painter n)
        (if (= n 0)
            painter
            (let ((up (up-split painter (- n 1)))
                 (right (right-split painter (- n 1))))
              (let ((top-left (beside up up))
                   (bottom-right (below right right))
                   (corner (corner-split painter (- n 1))))
                (beside (below painter top-left)
                        (below bottom-right corner))))))

(define (square-limit painter n)
        (let ((quarter (corner-split painter n)))
             (let ((half (beside (flip-horiz quarter) quarter)))
                  (below (flip-vert half) half))))

; --- More SVG code ---

(define (svg-document painter)
    (svg-start)
    (painter (make-frame (make-vect 0 0)
                         (make-vect 1 0)
                         (make-vect 0 1)))
    (svg-end))

(define draw-line svg-draw-line)

