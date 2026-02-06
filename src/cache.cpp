#include "texmaker.h"
#include <stdlib.h>
#include <string.h>

void TextureFile::addToCache(int id, const ppl7::grafix::Drawable &albedo, const ppl7::grafix::Drawable &normal,
                             const ppl7::grafix::Drawable &specular, const ppl7::grafix::Rect &r,
                             const ppl7::grafix::Point &pivot)
{
    CacheItem item;
    item.albedo = albedo.getDrawable(r);
    if (use_normal) item.normal = normal.getDrawable(r);
    if (use_specular) item.specular = specular.getDrawable(r);
    item.normal = normal.getDrawable(r);
    item.specular = specular.getDrawable(r);
    item.id = id;
    item.width = item.albedo.width();
    item.height = item.albedo.height();
    item.pivot = pivot;
    item.offset.x = r.x1;
    item.offset.y = r.y1;

    uint64_t key = ((uint64_t)item.height << 32) | ((uint64_t)item.width & 0xffff) << 16 | ((uint64_t)id & 0xffff);
    cache.insert(std::pair<uint64_t, CacheItem>(key, item));
}

void TextureFile::printCache() const
{
    printf("we have %zd sprites\n", cache.size());
    std::map<uint64_t, CacheItem>::const_reverse_iterator it;
    for (it = cache.rbegin(); it != cache.rend(); ++it) {
        printf("    id: %4d, height: %4d, width: %4d\n", (*it).second.id, (*it).second.height, (*it).second.width);
    }
    fflush(stdout);
}

void TextureFile::generateTexturesFromCache()
{
    // printf("Generating textures and index for %zd sprites\n", cache.size());
    std::map<uint64_t, CacheItem>::const_reverse_iterator it;
    size_t c = 0;
    for (it = cache.rbegin(); it != cache.rend(); ++it) {
        addToTexture(it->second);
        c++;
    }
    printf("%zd Sprites stored in %zd textures and %zd indexes\n", c, TextureList.size(), Index.size());

    fflush(stdout);
}

void TextureFile::addToTexture(const CacheItem &item)
{
    // Haben wir dafür Platz in einer vorhandenen Textur?
    ppl7::grafix::Rect tgt;
    // bool found=false;
    std::list<Texture>::iterator it;
    it = TextureList.begin();
    Texture *bestmatch = NULL;
    Texture *tx = NULL;
    ppl7::grafix::Size size(item.width, item.height);
    size_t smallest_size = 0;
    // ppl7::PrintDebug("Adding sprite id=%d size=%dx%d to texture\n", item.id, item.width, item.height);

    while (it != TextureList.end()) {
        tx = &(*it);
        size_t bytes = tx->findSmallestMatch(size);
        if ((smallest_size == 0) & (bytes > 0)) {
            smallest_size = bytes;
            bestmatch = tx;
        } else if (bytes > 0 && bytes < smallest_size) {
            smallest_size = bytes;
            bestmatch = tx;
        }
        ++it;
    }
    if (bestmatch) {
        if (!bestmatch->add(item.albedo, tgt)) {
            printf("ups\n");
            return;
        }
        tx = bestmatch;
    } else {
        // printf("Beginne neue Textur\n");
        tx = new Texture(twidth, theight);
        if (!tx->add(item.albedo, tgt)) {
            delete tx;
            // printf("Textur konnte nicht hinzugefuegt werden\n");
            return;
        }
        TextureList.push_back(*tx);
    }
    // Eintrag im Index machen
    IndexItem idx;
    idx.ItemId = item.id;
    idx.TextureId = tx->GetId();
    idx.pos = tgt;
    idx.pivot.x = item.pivot.x;
    idx.pivot.y = item.pivot.y;
    idx.offset.x = item.offset.x;
    idx.offset.y = item.offset.y;
    if (debug)
        printf("added sprite %4d to texture %2d in coordinates: %4d:%4d - %4d:%4d (%4d:%4d)\n", item.id, idx.TextureId,
               idx.pos.x1, idx.pos.y1, idx.pos.x2, idx.pos.y2, idx.pos.width(), idx.pos.height());
    Index.push_back(idx);

    if (use_normal) {
        auto it = NormalMap.find(idx.TextureId);
        if (it != NormalMap.end()) {
            it->second.blt(item.normal, tgt);
        } else {
            Texture *normal_tx = new Texture(twidth, theight, Texture::TextureType::Normal);
            normal_tx->blt(item.normal, tgt);
            NormalMap.insert(std::pair<int, Texture>(idx.TextureId, *normal_tx));
        }
    }
    if (use_specular) {
        auto it = SpecularMap.find(idx.TextureId);
        if (it != SpecularMap.end()) {
            it->second.blt(item.specular, tgt);
        } else {
            Texture *specular_tx = new Texture(twidth, theight, Texture::TextureType::Specular);
            specular_tx->blt(item.specular, tgt);
            SpecularMap.insert(std::pair<int, Texture>(idx.TextureId, *specular_tx));
        }
    }
}
