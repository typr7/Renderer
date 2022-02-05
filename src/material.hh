#ifndef _MATERIAL_HH_
#define _MATERIAL_HH_

#include <cstdint>
#include <memory>
#include "math_utils.hh"

static const uint64_t kPngSign = 0x89504e470d0a1a0a;

class Texture {
private:
    bool good_;
public:
    Texture(const std::string& texture_file_name);
    bool good() const noexcept { return good_; }
};

enum IllumMode {
    COLOR_ONLY = 0,
    DIFFUSE,
    PHONG
};

struct Material {
    static const Material default_material;

    std::string material_name;
    Vector3f ka;
    Vector3f kd;
    Vector3f ks;
    IllumMode illum_mode;
    std::shared_ptr<Texture> ambient_texture;
    std::shared_ptr<Texture> diffuse_texture;
};

inline const Material Material::default_material = {
    "Default Material",
    Vector3f::Zero(),
    Vector3f::Zero(),
    Vector3f::Zero(),
    IllumMode::PHONG,
    nullptr,
    nullptr
};

#endif // _MATERIAL_HH_