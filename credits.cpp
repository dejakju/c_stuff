/*
    $VER:       credits.cpp 1.3
    $DATE:      2023-05-02 (2025-11-15)
    $AUTHOR:    Goran (dejakju@gmail.com)

** Description: Writes a textfile to 'stdout' in a typewriter fashion
*/

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#include <unistd.h>

#define CREDITS_VERSION     1
#define CREDITS_REVISION    3


static void
show_usage(const char *argv0)
{
    std::cout << "Usage: "
        << argv0 << " [opt]"
        << std::endl
        << "\t -f \t.......... filename/path (default: credits.txt)"
        << std::endl
        << "\t -t \t.......... milliseconds (default: 25)"
        << std::endl
        << "\t -h \t.......... show usage"
        << std::endl
        << "\t -v \t.......... show version"
        << std::endl;
    exit(1);
}


static void
show_version(const char *argv0)
{
    std::cout << "\x1b[1;33m" << argv0 << "\x1b[1;30m"
        << " version " << CREDITS_VERSION << "." << CREDITS_REVISION
        << " (compiled: " << __DATE__ << ", " << __TIME__ << ")"
        << "\x1b[0m"
        << std::endl;
    exit(0);
}


static bool
is_numeric(std::string s)
{
    for (long unsigned int i = 0; i < s.length(); i++)
        if (isdigit(s[i]) == false)
            return false;
    return true;
}


/************************************* MAIN ENTRY POINT *************************************/
int 
main(int argc, char* argv[])
{
    int opt;                                    // the current option
    std::string fileName = "credits.txt";       // set the value of the -f parameter (default: credits.txt)
    std::chrono::milliseconds mSeconds{25};     // set the value of the -t parameter (default: 25ms)

    // Main parameter/options loop with some validation
    while ((opt = getopt(argc, argv, "hHf:t:vV")) != -1)
    {
        switch (opt)
        {
            case 'f':
            {
                // If the filename is less than one character, quit
                // optarg hols the pointer to the argument of the current option
                std::istringstream iss(optarg);
                if (iss.str().length() < 1)
                    show_usage(argv[0]);

                iss >> fileName;
            } break;

            case 't':
            {

                std::istringstream iss(optarg);
                if (!is_numeric(iss.str()))
                {
                    std::cout << "ERROR: -t parameter needs to be a valid integer" << std::endl;
                    show_usage(argv[0]);
                }
                else
                {
                    int number;
                    iss >> number;
                    mSeconds = std::chrono::milliseconds(number);
                }
            } break;

            case 'H':
            case 'h':
            {
                show_usage(argv[0]);
            } break;

            case 'V':
            case 'v':
            {
                show_version(argv[0]);
            } break;

            default:
                show_usage(argv[0]);
        }
    }

    // If there are any (unknown) additional arguments, bail out
    // optind is the index of the first __non-option__ in argv
    if (optind != argc)
    {
        std::cerr << "ERROR: wrong number of arguments" << std::endl;
        show_usage(argv[0]);
    }

    // If we reach this point, almost all checks are done
    std::ifstream file;
    file.open(fileName);

    // Check for a valid file, if it fails, bail out
    if (file.fail())
    {
        std::cerr << "ERROR: could not open file \'" << fileName << "\'" << std::endl;
        exit(1);
    }

    // Read all contents of the file into a 'std::stringstream' buffer and convert it to a 'std::string'
    std::stringstream buffer;
    std::string file_content;
    buffer << file.rdbuf();
    file_content = buffer.str();

    // Traverse the whole string step by step and put the main thread to sleep for the specified amount of time
    for (long unsigned int i = 0; i < file_content.length(); i++)
    {
        std::cout << file_content[i];
        std::this_thread::sleep_for(mSeconds);
        // This next line is very(!) important, because STDIN & STDOUT are buffered!
        std::cout.flush();
    }

    // Add a newline character at the very end, if neccessary
    if (file_content.at(file_content.length() - 1) != '\n')
        std::cout << std::endl;

    // We're done, so close the file and return with a smile on your face
    file.close();
    return 0;
}
