        #include <cmrc/cmrc.hpp>
#include <map>
#include <utility>

namespace cmrc {
namespace test_resources {

namespace res_chars {
// These are the files which are available in this resource library
// Pointers to res/65C02_extended_opcodes_test.bin
extern const char* const f_5cff_res_65C02_extended_opcodes_test_bin_begin;
extern const char* const f_5cff_res_65C02_extended_opcodes_test_bin_end;
// Pointers to res/65C02_extended_opcodes_test.lst
extern const char* const f_b7f1_res_65C02_extended_opcodes_test_lst_begin;
extern const char* const f_b7f1_res_65C02_extended_opcodes_test_lst_end;
// Pointers to res/6502_functional_test.bin
extern const char* const f_3dd1_res_6502_functional_test_bin_begin;
extern const char* const f_3dd1_res_6502_functional_test_bin_end;
// Pointers to res/6502_functional_test.lst
extern const char* const f_83f5_res_6502_functional_test_lst_begin;
extern const char* const f_83f5_res_6502_functional_test_lst_end;
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
    static auto f_9b20_res_dir = root_directory_dir.directory.add_subdir("res");
    root_index.emplace("res", &f_9b20_res_dir.index_entry);
    root_index.emplace(
        "res/65C02_extended_opcodes_test.bin",
        f_9b20_res_dir.directory.add_file(
            "65C02_extended_opcodes_test.bin",
            res_chars::f_5cff_res_65C02_extended_opcodes_test_bin_begin,
            res_chars::f_5cff_res_65C02_extended_opcodes_test_bin_end
        )
    );
    root_index.emplace(
        "res/65C02_extended_opcodes_test.lst",
        f_9b20_res_dir.directory.add_file(
            "65C02_extended_opcodes_test.lst",
            res_chars::f_b7f1_res_65C02_extended_opcodes_test_lst_begin,
            res_chars::f_b7f1_res_65C02_extended_opcodes_test_lst_end
        )
    );
    root_index.emplace(
        "res/6502_functional_test.bin",
        f_9b20_res_dir.directory.add_file(
            "6502_functional_test.bin",
            res_chars::f_3dd1_res_6502_functional_test_bin_begin,
            res_chars::f_3dd1_res_6502_functional_test_bin_end
        )
    );
    root_index.emplace(
        "res/6502_functional_test.lst",
        f_9b20_res_dir.directory.add_file(
            "6502_functional_test.lst",
            res_chars::f_83f5_res_6502_functional_test_lst_begin,
            res_chars::f_83f5_res_6502_functional_test_lst_end
        )
    );
    return root_index;
}

}

cmrc::embedded_filesystem get_filesystem() {
    static auto& index = get_root_index();
    return cmrc::embedded_filesystem{index};
}

} // test_resources
} // cmrc
    