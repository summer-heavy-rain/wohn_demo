set(INCLUDE "." "./ui/custom" "./ui/generated" "./ui/generated/guider_customer_fonts")
file(GLOB_RECURSE SOURCES ./*.c)
#list(FILTER SOURCES EXCLUDE REGEX ".*/lvgl_display\\.c$")
list(FILTER SOURCES EXCLUDE REGEX ".*/img_exporter\\.c$")
list(FILTER SOURCES EXCLUDE REGEX ".*/ui/generated/images/.*\\.c$")

idf_component_register(
    SRCS ${SOURCES}
    INCLUDE_DIRS ${INCLUDE}
    REQUIRES lvgl esp_lvgl_port components
)

