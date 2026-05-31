//
//  main.cpp
//  sprconv
//
//  BSD 3-Clause License
//
//  Copyright (c) 2026, John Othwolo
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice, this
//     list of conditions and the following disclaimer.

//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//  3. Neither the name of the copyright holder nor the names of its
//     contributors may be used to endorse or promote products derived from
//     this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
//  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
//  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//  Created by John Othwolo on 5/29/26.
//

#include <iostream>
#include <fstream>
#include <string>

#include "spr/spr.h"

#define NANOSVG_ALL_COLOR_KEYWORDS    // Include full list of color keywords.
#define NANOSVG_IMPLEMENTATION        // Expands implementation
#define NANOSVGRAST_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "nanosvg.h"
#include "nanosvgrast.h"
#include "stb_image_write.h"

static size_t get_ifile_size(std::ifstream &file)
{
    size_t cur, size;
    cur = file.tellg();
    file.seekg(0, std::fstream::seekdir::end);
    size = file.tellg();
    file.seekg(cur);
    return size;
}

static const char* read_spr_file(const char *filepath)
{
    std::ifstream file;
    size_t filesize;
    char* filedata;
    
    file = std::ifstream(filepath, std::ios::binary | std::ios::in);
    filesize = get_ifile_size(file);
    filedata = new char[filesize];
    file.read(filedata, filesize);
    file.close();
    
    return filedata;
}

static const char *get_fullpath(const char *patharg)
{
    char rpath[PATH_MAX] = {0};
    // get full path if path is relative
    return (*patharg != '/') ? realpath(patharg, rpath) : patharg;
}

static NSVGimage* nsvgFromSpr(std::unique_ptr<SprDocument> const &sprdoc)
{
    const std::string& svgoutstr = sprdoc->drawSvg();
    return nsvgParse((char*)svgoutstr.c_str(), "px", sprdoc->header.base.dimensions.width);
}

static const char* generate_outpath(const char *inputpath, const char *newExt)
{
    static char outfilepath[PATH_MAX]; // static so that we can return it
    const char *bnp; // bnp = basename pointer
    const char *extp; // file-extension pointer
    
    bnp = strrchr(inputpath, '/');
    assert(bnp); bnp++; // advance pointer past forward slash
    
    strncpy(outfilepath, inputpath, std::min(sizeof(outfilepath), (size_t)(bnp - inputpath)));
    extp = strchr(bnp, '.');
    strncpy(&outfilepath[(bnp - inputpath)], bnp, std::min((size_t)(extp - bnp), sizeof(outfilepath) - (size_t)(bnp - inputpath)));
    strncat(outfilepath, ".0.", sizeof(outfilepath) - ((extp - bnp + 1) + (bnp - inputpath)));
    strncat(outfilepath, newExt, sizeof(outfilepath) - ((extp - bnp + 1) + (bnp - inputpath) + sizeof(".0.") - 1));

    return outfilepath;
}

static std::shared_ptr<char> rasterize_svg(NSVGimage* image)
{
    NSVGrasterizer *rast = NULL;
    std::shared_ptr<char> img;
    
    if ((rast = nsvgCreateRasterizer()) == NULL)
        throw "Could not init rasterizer.\n";
    
    // rasterizer zeroes out the buffer
    if ((img = std::make_shared<char>(image->width * image->height * 4)) == NULL)
        throw "Could not alloc image buffer.\n";
    
    nsvgRasterize(rast, image, 0,0,1, (unsigned char*) img.get(), (int)image->width, (int)image->height, (int)image->width * 4);
    nsvgDeleteRasterizer(rast);
    return img;
}

static void get_options(int argc, const char * argv[], const char **inpath, bool &ispng, bool &isbmp, bool &issvg)
{
    *inpath = NULL;
    
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-')
        {
            if (strncmp(&argv[i][1], "svg", sizeof("spr") - 1) == 0)
                issvg = true;
            else if (strncmp(&argv[i][1], "png", sizeof("spr") - 1) == 0)
                ispng = true;
            else if (strncmp(&argv[i][1], "bmp", sizeof("spr") - 1) == 0)
                isbmp = true;
            else goto _Fail_Exit;
        }
        else if (*inpath == NULL)
        {
            *inpath = argv[i];
        }
        else goto _Fail_Exit;
    }

    if ((!isbmp && !ispng && !issvg) || *inpath == NULL){
_Fail_Exit:
        fprintf(stderr, "usage sprconv: [-png|-svg|-bmp] <input-spr>\n");
        exit(EINVAL);
    }
}

int main(int argc, const char * argv[]) {
    bool ispng, isbmp, issvg = isbmp = issvg = false;
    std::unique_ptr<SprDocument> sprdoc;
    std::shared_ptr<char> imgdata;
    const char *filepath;
    NSVGimage* image;
    
    get_options(argc, argv, &filepath, ispng, isbmp, issvg);
    
    filepath = get_fullpath(filepath);
    
    printf("parsing spr\n");
    sprdoc = std::make_unique<SprDocument>(read_spr_file(filepath));
    
    printf("generating SVG.\n");
    if ((image = nsvgFromSpr(sprdoc)) == NULL)
        throw "Could not parse generated SVG output.\n";
    
    printf("rasterizing image %d x %d\n", (int)image->width, (int)image->height);
    imgdata = rasterize_svg(image);
    
    if (issvg)
    {
        printf("writing to svg\n");
        auto outfile = std::ofstream(generate_outpath(filepath, "svg"), std::ios::out);
        outfile.clear();
        outfile << sprdoc->drawSvg();
        outfile.close();
    }
    
    if (isbmp)
    {
        printf("writing to bmp\n");
        stbi_write_bmp(generate_outpath(filepath, "bmp"), (int)image->width, (int)image->height, 4, imgdata.get());
    }
    
    if (ispng)
    {
        printf("writing to png\n");
        stbi_write_png(generate_outpath(filepath, "png"), (int)image->width, (int)image->height, 4, imgdata.get(), (int)image->width * 4);
    }
    
    nsvgDelete(image);
    
    return 0;
}
