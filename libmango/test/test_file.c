#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <check.h>
#include <string.h>
#include "../libmango.h"

START_TEST(test_compress) {
    void *file;
    void *img;
    file = new_mango_file();
    mangofile_add_image_by_path(file, "test.jpg");

    img = mangofile_get_image(file, 0);
    mangoimg_compress(img, "GZIP");

    img = mangofile_get_image(file, 0);
    char * compression = mangoimgmeta_compression(mangoimg_get_meta(img));
    ck_assert(strcmp(compression, "GZIP") == 0);
}
END_TEST

START_TEST(test_set_null) {
    void * file;
    void * meta;
    char * author = NULL;
    file = new_mango_file();
    meta = mangofile_get_meta(file);

    mangometa_set_author(meta, "test");
    mangometa_set_author(meta, author);

    ck_assert(mangometa_get_author(meta) == NULL );
}
END_TEST

START_TEST(test_set_title) {
    void * file;
    void * meta;

    file = new_mango_file();
    meta = mangofile_get_meta(file);

    mangometa_set_title(meta, "test");

    ck_assert(strcmp(mangometa_get_title(meta), "test")== 0);
}
END_TEST

START_TEST(test_set_author) {
    void * file;
    void * meta;

    file = new_mango_file();
    meta = mangofile_get_meta(file);

    mangometa_set_author(meta, "test");

    ck_assert(strcmp(mangometa_get_author(meta), "test") == 0);
}
END_TEST

START_TEST(test_set_publisher) {
    void * file;
    void * meta;

    file = new_mango_file();
    meta = mangofile_get_meta(file);

    mangometa_set_publisher(meta, "test");

    ck_assert(strcmp(mangometa_get_publisher(meta), "test") == 0);
}
END_TEST

START_TEST(test_set_translation) {
    void * file;
    void * meta;

    file = new_mango_file();
    meta = mangofile_get_meta(file);

    mangometa_set_translation(meta, "test");

    ck_assert(strcmp(mangometa_get_translation(meta), "test") == 0);
}
END_TEST

START_TEST(test_set_source) {
    void * file;
    void * meta;

    file = new_mango_file();
    meta = mangofile_get_meta(file);

    mangometa_set_source(meta, "test");

    ck_assert(strcmp(mangometa_get_source(meta), "test") == 0);
}
END_TEST

// Save
START_TEST(test_save) {
    MangoFile file;
    MangoImage img;

    file = new_mango_file();
    mangofile_add_image_by_path(file, "test.jpg");

    mangofile_save(file, "testfile.mango");

    // check if the file was created
    FILE * created_file;
    created_file = fopen("testfile.mango", "r");

    ck_assert(created_file != NULL);
}
END_TEST

START_TEST(test_save_json) {
    MangoFile file;
    MangoImage img;

    file = new_mango_file();
    mangofile_add_image_by_path(file, "test.jpg");

    mangofile_save_json(file, "testfile.json");

    // check if the file was created
    FILE * created_file;
    created_file = fopen("testfile.json", "r");

    ck_assert(created_file != NULL);
}
END_TEST

Suite * mango_suite(void) {
    Suite *s;
    TCase *tc_core;
    TCase *tc_io;


    s = suite_create("Mango");

    tc_core = tcase_create("Core");
    tc_io = tcase_create("IO");

    // Core
    tcase_add_test(tc_core, test_compress);
    tcase_add_test(tc_core, test_set_null);
    tcase_add_test(tc_core, test_set_title);
    tcase_add_test(tc_core, test_set_author);
    tcase_add_test(tc_core, test_set_publisher);

    tcase_add_test(tc_core, test_set_source);
    tcase_add_test(tc_core, test_set_translation);

    // IO
    tcase_add_test(tc_io, test_save);
    tcase_add_test(tc_io, test_save_json);

    // add cases to suites
    suite_add_tcase(s, tc_core);
    suite_add_tcase(s, tc_io);

    return s;
}


int main(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = mango_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}