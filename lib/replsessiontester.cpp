/*
 *  Subs Scheme Interpreter
 *  Copyright (C) 2010 Andy Balaam
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
**/

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "replsessiontester.h"
#include "subsrepl.h"

using namespace std;

namespace
{

int check_sessions_identical( const string& input, const string& actual_output,
    const string& expected_output )
{
    if( actual_output == expected_output )
    {
        return 0;
    }

    // TODO: better error message
    cerr << "subs: Error: session-test failed since:"
        << endl << "> " << input << endl
        << "produced this output:"
        << endl << actual_output
        << "but this was expected:"
        << endl << expected_output;

    return 1;
}

int process_file_loop( const string& firstline, istream& instream,
    SubsRepl& repl )
{
    int ret = 0;

    string inputline = firstline;
    string expected;

    while( ret == 0 && !instream.eof() )
    {
        string line;
        getline( instream, line );

        bool run_now = false;

        if( instream.eof() )
        {
            if( !line.empty() )
            {
                expected += line;
                expected += '\n';
            }
            run_now = true;
        }
        else if( line.substr( 0, 2 ) == "> " )
        {
            run_now = true;
        }

        if( run_now )
        {
            stringstream input_stream;
            ostringstream output_stream;
            input_stream << inputline;

            repl.Run( input_stream, output_stream, output_stream );

            ret = check_sessions_identical( inputline,
                output_stream.str(), expected );

            if( line.size() > 1 )
            {
                inputline = line.substr( 2 );
            }

            expected.clear();
        }
        else if( line.substr( 0, 2 ) == ". " )
        {
            inputline += line.substr( 2 );
            inputline += '\n';
        }
        else if( !line.empty() )
        {
            expected += line;
            expected += '\n';
        }
    }

    return ret;
}

int get_first_input_line( istream& instream, const string& filename,
    string& retinputline )
{
    while( !instream.eof() && retinputline.empty() )
    {
        getline( instream, retinputline );
    }

    // It was nothing but empty lines...
    if( retinputline.empty() )
    {
        cerr << "subs: Error: empty file '" << filename
            << "' in session-test." << endl;
        return 1;
    }

    if( retinputline.substr( 0, 2 ) != "> " )
    {
        cerr << "subs: Error: unexpected line found: '"
            << retinputline << "'." << endl;
        return 1;
    }

    retinputline = retinputline.substr( 2 );

    return 0;
}

int process_file( istream& instream, SubsRepl& repl, const string& filename )
{
    string inputline;
    int ret = get_first_input_line( instream, filename, inputline );

    if( ret == 0 )
    {
        return process_file_loop( inputline, instream, repl );
    }
    else
    {
        return ret;
    }
}

}

int ReplSessionTester::Run( const std::vector<std::string>& filenames )
{
    SubsRepl repl( false, false );

    int ret = 0;
    for( vector<string>::const_iterator it = filenames.begin();
        it != filenames.end(); ++it )
    {
        ifstream instream( it->c_str() );

        if( !instream.good() )
        {
            cerr << "subs: Error: unable to open file '"
                << *it << "'." << endl;
            ret = 1;
            break;
        }

        ret = process_file( instream, repl, *it );

        if( ret != 0 )
        {
            break;
        }
    }
    return ret;
}


