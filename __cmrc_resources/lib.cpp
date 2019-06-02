        #include <cmrc/cmrc.hpp>
#include <map>
#include <utility>

namespace cmrc {
namespace resources {

namespace res_chars {
// These are the files which are available in this resource library
// Pointers to rom/kernal.rom
extern const char* const f_8043_rom_kernal_rom_begin;
extern const char* const f_8043_rom_kernal_rom_end;
// Pointers to rom/basic.rom
extern const char* const f_4225_rom_basic_rom_begin;
extern const char* const f_4225_rom_basic_rom_end;
// Pointers to rom/chargen.rom
extern const char* const f_a199_rom_chargen_rom_begin;
extern const char* const f_a199_rom_chargen_rom_end;
// Pointers to res/font.ttf
extern const char* const f_8481_res_font_ttf_begin;
extern const char* const f_8481_res_font_ttf_end;
}

namespace {

const cmrc::detail::index_type&
get_root_index() {
    static cmrc::detail::directory root_directory_;
    static cmrc::detail::file_or_directory root_directory_fod{root_directory_};
    static cmrc::detail::index_type root_index;
    root_index.emplace("", &root_directory_fod);
    struct dir_inl {
        class cmrc::detail::directory& directory;
    };
    dir_inl root_directory_dir{root_directory_};
    (void)root_directory_dir;
    static auto f_5f39_rom_dir = root_directory_dir.directory.add_subdir("rom");
    root_index.emplace("rom", &f_5f39_rom_dir.index_entry);
    static auto f_9b20_res_dir = root_directory_dir.directory.add_subdir("res");
    root_index.emplace("res", &f_9b20_res_dir.index_entry);
    root_index.emplace(
        "rom/kernal.rom",
        f_5f39_rom_dir.directory.add_file(
            "kernal.rom",
            res_chars::f_8043_rom_kernal_rom_begin,
            res_chars::f_8043_rom_kernal_rom_end
        )
    );
    root_index.emplace(
        "rom/basic.rom",
        f_5f39_rom_dir.directory.add_file(
            "basic.rom",
            res_chars::f_4225_rom_basic_rom_begin,
            res_chars::f_4225_rom_basic_rom_end
        )
    );
    root_index.emplace(
        "rom/chargen.rom",
        f_5f39_rom_dir.directory.add_file(
            "chargen.rom",
            res_chars::f_a199_rom_chargen_rom_begin,
            res_chars::f_a199_rom_chargen_rom_end
        )
    );
    root_index.emplace(
        "res/font.ttf",
        f_9b20_res_dir.directory.add_file(
            "font.ttf",
            res_chars::f_8481_res_font_ttf_begin,
            res_chars::f_8481_res_font_ttf_end
        )
    );
    return root_index;
}

}

cmrc::embedded_filesystem get_filesystem() {
    static auto& index = get_root_index();
    return cmrc::embedded_filesystem{index};
}

} // resources
} // cmrc
    