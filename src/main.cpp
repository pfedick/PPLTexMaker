#include <ppl7.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "texmaker.h"
#include "version.h"

void help()
{
    printf("PPL TexMaker Version %s.%s\n"
           "\n"
           "Optionen:\n"
           "   -s DIR   source directory\n"
           "   -t FILE  target file\n"
           "   -f FILE  json file with sprite data (optional)\n"
           "   -w #     width of textures in the file in pixel (default=512)\n"
           "   -h #     height of textures in the file in pixel (default=width)\n"
           "   -i       take index from filename"
           "   -pd      method of pivot point detection: fixed|bricks|lower_middle|lower_left\n"
           "            when using \"params\", the parameter -pp or -px and -py are mandatory\n"
           "   -pp x,y  pivot-point for all sprites (default=0,0)\n"
           "   -px #    pivot-point for all sprites, x-coordinate (default=0)\n"
           "   -py #    pivot-point for all sprites, y-coordinate (default=0)\n"
           "   -a TEXT  name of authors (optional)\n"
           "   -c TEXT  Copyright-String (optional)\n"
           "   -n TEXT  Name der Datei (optional)\n"
           "   -d TEXT  Description (optional)\n"
           "   -x FILE  exports all textures as png files with this prefix\n"
           "   --help   shows this help\n",
           TEXMAKER_VERSION, TEXMAKER_REVSION);
}

int loadFromDirectory(const ppl7::String &source, int px, int py, TextureFile &Tex)
{
    ppl7::String basedir = ppl7::File::getPath(source);
    if (ppl7::File::exists(basedir + "/normal")) {
        Tex.EnableNormal(basedir + "/normal");
    }
    if (ppl7::File::exists(basedir + "/specular")) {
        Tex.EnableSpecular(basedir + "/specular");
    }
    if (ppl7::File::exists(basedir + "/albedo")) {
        Tex.EnableAlbedo(basedir + "/albedo");
        basedir = basedir + "/albedo";
    } else {
        Tex.EnableAlbedo(basedir);
    }

    printf("basedir: %s\n", (const char *)basedir);
    int id = 0;
    ppl7::Dir Dir;
    ppl7::String Path, Pattern;
    Path = basedir;
    Pattern = "*.png";

    // printf("Path: %s, Pattern: %s\n", (const char *)basedir, (const char *)Pattern);
    Dir.open(basedir, ppl7::Dir::SORT_FILENAME);
    ppl7::Dir::Iterator it;
    Dir.reset(it);
    ppl7::DirEntry Entry;
    while (Dir.getNextPattern(Entry, it, Pattern)) {
        printf("Found: %s\n", (const char *)(Entry.File));
        if (!Tex.AddFile(Entry.File, id, px, py)) {
            return 1;
        }
        id++;
    }
    return 0;
}

int loadFromFile(const ppl7::String &source, int px, int py, TextureFile &Tex, int sx, int sy)
{
    int id = 0;
    if (sx <= 0 || sy <= 0) {
        printf("ERROR: parameters -sx and -sy are required!\n");
        return 1;
    }
    ppl7::grafix::Image img;
    ppl7::grafix::Point p;
    ppl7::grafix::Size s(sx, sy);

    img.load(source);
    for (p.y = 0; p.y < img.height(); p.y += sy) {
        for (p.x = 0; p.x < img.width(); p.x += sx) {
            ppl7::grafix::Drawable d = img.getDrawable(p, s);
            if (!Tex.AddSurface(d, d, d, NULL, id, px, py)) {
                printf("Could not add surface to Texture\n");
                return 1;
            }
            id++;
        }
    }
    return 0;
}

void loadFromListfile(const char *listfile, TextureFile &Tex)
{
    ppl7::File ff(listfile);
    ppl7::String basedir = ppl7::File::getPath(listfile);
    try {
        while (!ff.eof()) {
            ppl7::String line = ff.gets(1024);
            ppl7::Array Token(line, ",");
            if (Token.size() != 4) {
                throw ppl7::Exception("ERROR: Ungueltige Anzahl Token");
            }
            int id = Token[0].toInt();
            ppl7::String file = basedir + "/" + Token[1];
            int px = Token[2].toInt();
            int py = Token[2].toInt();
            // printf("Found: %s, ", (const char*)(file));
            if (!Tex.AddFile(file, id, px, py)) {
                throw ppl7::Exception("Debug 3");
            }
        }
    }
    catch (...) {
    }
}

int main(int argc, char **argv)
{
    ppl7::String listfile = ppl7::GetArgv(argc, argv, "-f");
    ppl7::String source = ppl7::GetArgv(argc, argv, "-s");
    ppl7::String target = ppl7::GetArgv(argc, argv, "-t");
    ppl7::String savepng = ppl7::GetArgv(argc, argv, "-x");
    ppl7::String pivot_detection = ppl7::GetArgv(argc, argv, "--pivot_detection");

    if (ppl7::HaveArgv(argc, argv, "--help") || (source.isEmpty() && listfile.isEmpty()) || target.isEmpty() ||
        argc < 2) {
        help();
        return 0;
    }
    if (source.notEmpty() && listfile.notEmpty()) {
        printf("ERROR: You cannot use -s and -f at the same time!\n");
        help();
        return 1;
    }

    ppl7::grafix::Grafix Grafix;

    TextureFile Tex;

    ppl7::String Tmp;
    Tmp = ppl7::GetArgv(argc, argv, "-a");
    if (Tmp.notEmpty()) Tex.SetAuthor(Tmp);
    Tmp = ppl7::GetArgv(argc, argv, "-c");
    if (Tmp.notEmpty()) Tex.SetCopyright(Tmp);
    Tmp = ppl7::GetArgv(argc, argv, "-n");
    if (Tmp.notEmpty()) Tex.SetName(Tmp);
    Tmp = ppl7::GetArgv(argc, argv, "-d");
    if (Tmp.notEmpty()) Tex.SetDescription(Tmp);

    if (pivot_detection.notEmpty()) {
        // printf("test: %s\n", (const char*)pivot_detection);
        if (pivot_detection == "params")
            Tex.SetPivotDetection(PIVOT_PARAMS);
        else if (pivot_detection == "bricks")
            Tex.SetPivotDetection(PIVOT_BRICKS);
        else if (pivot_detection == "lower_middle")
            Tex.SetPivotDetection(PIVOT_LOWER_MIDDLE);
        else if (pivot_detection == "lower_left")
            Tex.SetPivotDetection(PIVOT_LOWER_LEFT);
        else {
            printf("ERROR: unknwon pivot_detection algorithm [%s]\n", (const char *)pivot_detection);
            return (1);
        }
    }

    int w, h, px, py, sx, sy;
    w = ppl7::GetArgv(argc, argv, "-w").toInt();
    h = ppl7::GetArgv(argc, argv, "-h").toInt();
    px = ppl7::GetArgv(argc, argv, "-px").toInt();
    py = ppl7::GetArgv(argc, argv, "-py").toInt();
    sx = ppl7::GetArgv(argc, argv, "-sx").toInt();
    sy = ppl7::GetArgv(argc, argv, "-sy").toInt();

    if (w > 0 && h == 0) h = w;
    if (h > 0 && w == 0) w = h;
    if (w > 0) Tex.SetTextureSize(w, h);

    if (source.notEmpty()) {
        if (ppl7::File::exists(source)) {
            ppl7::DirEntry d = ppl7::File::statFile(source);
            if (d.isFile()) {
                printf("load from file: source=%s\n", (const char *)source);
                int ret = loadFromFile(source, px, py, Tex, sx, sy);
                if (ret != 0) return ret;
            } else if (ppl7::File::isDir(source)) {
                printf("load from directory: source=%s\n", (const char *)source);
                int ret = loadFromDirectory(source, px, py, Tex);
                if (ret != 0) return ret;
            } else {
                printf("ERROR: source is not a file or directory [%s]\n", (const char *)source);
                return 1;
            }
        } else {
            printf("ERROR: source does not exist [%s]\n", (const char *)source);
            return 1;
        }
    } else if (listfile.notEmpty()) {
        try {
            loadFromListfile(listfile, Tex);
        }
        catch (const ppl7::Exception &ex) {
            ex.print();
            return 1;
        }
    } else {
        printf("ERROR: Don't know what to do!\n");
        return 1;
    }

    try {
        Tex.Save(target);
    }
    catch (const ppl7::Exception &ex) {
        ex.print();
        return 1;
    }
    if (savepng.notEmpty()) {
        printf("saving texturen...\n");
        Tex.SaveTextures(savepng);
    }
    return 0;
}
