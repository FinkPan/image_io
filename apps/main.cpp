#include "image_io.hpp"


#  define EQUAL(a,b)              (_stricmp(a,b)==0)

static void Usage()		//Usage
{
    printf(
        "\n\nUsage: image_io [-in inputfile] [-ou [-b] outfile]\n"
        "-in : inputfile.\n"
        "-ou : outfile\n"
        "-b : write outfile by binary.\n"
        );
}

int main(int argc, char **argv)
{

    const char* input_file_name = NULL;
    const char* output_file_name = NULL;
    bool binary_file = false;

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

        }
        else
        {
            i = argc;
            Usage();
        }
    }
    if (!input_file_name == NULL && !output_file_name == NULL)
    {
        std::string fileformat;
        std::ifstream filedata;

        fileformat = ImageLoad(input_file_name,filedata);

        if (fileformat == "P1" || fileformat == "P4")
        {
            
            Pbm mypbm(filedata);
            
            
            mypbm.WriteImage(output_file_name,binary_file);
        }
        else if (fileformat == "P2" || fileformat == "P5")
        {
           Pgm mypgm(filedata);
           mypgm.WriteImage(output_file_name,binary_file);
        }
        else if (fileformat == "P3" || fileformat == "P6")
        {
           Ppm myppm(filedata);
           myppm.WriteImage(output_file_name,binary_file);
        }
        else
        {
            std::cout << "无法读取文件或者文件格式非法!" << std::endl;
        }
        filedata.close();
    }
    else
        Usage();

    return 0;
}