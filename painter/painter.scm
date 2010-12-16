
; --- Standard libraries ---

; TODO: these will be provided with subs at some point

;(define (begin a b)
;    a
;    b)

(define (for-each func items)
    (if (null? items)
        nil
        (cons (func (car items))
               (for-each func (cdr items)))))
; TODO: use begin.  Don't know why it currently crashes interpreter

(define (make-vector x y)
    (cons x y))

(define (xcor-vect vec)
    (car vec))

(define (ycor-vect vec)
    (cdr vec))

(define (add-vect v1 v2)
    (make-vector (+ (xcor-vect v1) (xcor-vect v2))
                 (+ (ycor-vect v1) (ycor-vect v2))))

(define (scale-vect s v)
    (make-vector (* s (xcor-vect v))
                 (* s (ycor-vect v))))

; --- SVG specifics ---

(define (svg-start)
    (display "<svg xmlns:svg='http://www.w3.org/2000/svg' xmlns='http://www.w3.org/2000/svg' width='100' height='100'>")
    (newline))

(define (svg-draw-line v1 v2)
    (display "  <path d='M ")
    (display (* 100 (xcor-vect v1)))
    (display " ")
    (display (* 100 (ycor-vect v1)))
    (display " ")
    (display (* 100 (xcor-vect v2)))
    (display " ")
    (display (* 100 (ycor-vect v2)))
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

; --- More SVG code ---

(define (svg-document painter)
    (svg-start)
    (painter (make-frame (make-vector 0 0)
                         (make-vector 0 1)
                         (make-vector 1 0)))
    (svg-end))

(define draw-line svg-draw-line)

; --- Rendering ---

; Exercise 2.49 - outline of the frame

(define outliner
    (segments->painter
        (list
            (make-segment (make-vector 0 0) (make-vector 1 0))
            (make-segment (make-vector 1 0) (make-vector 1 1))
            (make-segment (make-vector 1 1) (make-vector 0 1))
            (make-segment (make-vector 0 1) (make-vector 0 0)))))

(svg-document outliner)




;(draw-line (make-segment (make-vector 0.25 0.25) (make-vector 0.50 0.50)))


