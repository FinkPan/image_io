#include "image_io.hpp"
#include <iostream>

#  define EQUAL(a,b)              (stricmp(a,b)==0)

static void Usage()		//Usage
{
    printf(
        "Usage: image_io [-in inputfile] [-ou [-b] outfile]\n"
        "-in : inputfile.\n"
        "-ou : outfile\n"
        "-b : write outfile by binary.\n"
        );
}

int main(int argc, char **argv)
{
    image_io mypbm;
    const char* pszInFile = NULL;
    const char* pszOutFile = NULL;
    bool bBinaryFile = false;

    if(argc == 1)
        Usage();

    for (int i = 1; i < argc; i++)
    {
        if (EQUAL(argv[i],"-in"))
        {
            pszInFile = argv[++i];
        }
        else if (EQUAL(argv[i],"-ou"))
        {
            if ((EQUAL(argv[++i],"-b")))
            {
                bBinaryFile = true;
                pszOutFile = argv[++i];
            }
            else
                pszOutFile = argv[i];

        }
        else
        {
            i = argc;
            Usage();
        }
    }
    if (!pszInFile == NULL && !pszOutFile == NULL)
    {
        if (mypbm.ReadFile(pszInFile))
             mypbm.WriteFile(pszOutFile,bBinaryFile);
    }
    else
        std::cout << "输入参数有错." << std::endl;

    return 0;
}