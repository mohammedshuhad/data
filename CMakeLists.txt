idf_component_register(
    SRCS "src/Data.cpp" "src/NvsHandler.cpp"
    INCLUDE_DIRS "include"
    REQUIRES nvs_flash
)