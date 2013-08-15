#include "image_io.hpp"
#  define EQUAL(a,b)              (_stricmp(a,b)==0)

static void Usage()		//Usage
{
    printf(
        "\n\nUsage: PgmTest [-in inputfile] [-ou [-b] outfile]\n"
        "PgmTest [-f inputfile]\n"
        "-in : inputfile.\n"
        "-ou : outfile\n"
        "-b : write outfile by binary.\n"
        "-f : print the image information.\n"
        );
}

int main(int argc, char **argv)
{

    const char* input_file_name = NULL;
    const char* output_file_name = NULL;
    bool binary_file = false;
    Pgm mypgm;

    for (int i = 1; i < argc; i++)
    {
        if (EQUAL(argv[i],"-in"))
        {
            input_file_name = argv[++i];
        }
        else if (EQUAL(argv[i],"-ou"))
        {
            if ((EQUAL(argv[++i],"-b")))
            {
                binary_file = true;
                output_file_name = argv[++i];
            }
            else
                output_file_name = argv[i];
            if (!input_file_name == NULL && !output_file_name == NULL)
            {
                mypgm.ReadImage(input_file_name);
                mypgm.WriteImage(output_file_name,binary_file);
            }

        }
        else if (EQUAL(argv[i],"-f"))
        {
            input_file_name = argv[++i];
            if (!input_file_name == NULL)
            {
                mypgm.ReadImage(input_file_name);
                std::cout << "\nFile Format: "<< mypgm.GetFileFormat()
                          << "\nWidth: "      << mypgm.GetWidth()
                          << "\nHeight: "     << mypgm.GetHeight()
                          << "\nMaxValue: "   << mypgm.GetMaxValue();

            }
        }
        else
        {
            i = argc;
            Usage();
        }
    }
    if (1 == argc)
    {
        Usage();
    }
    return 0;
}
