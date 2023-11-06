from conans import ConanFile, CMake, tools
from conans.tools import os_info


class Connect4Conan(ConanFile):
    name = "connect-4"
    version = "0.1"
    author = ""
    description = ""
    settings = "os", "compiler", "build_type", "arch"
    requires = [
        "qt/6.6.0",
    ]
    default_options = {
        "qt:shared": True,
        "qt:openssl": False,
        "qt:with_brotli": False,
        "qt:with_harfbuzz" : False,
        "qt:with_libpng" : False,
        "qt:with_md4c" : False,
        "qt:with_odbc" : False,
        "qt:with_openal" : False,
        "qt:with_pq" : False,
        "qt:with_sqlite3" : False,
    }
    generators = "cmake_find_package", "cmake_paths"

    def build_requirements(self):
        self.test_requires("gtest/1.12.1")

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.definitions["CMAKE_BUILD_TYPE"] = self.settings.build_type.upper()
        cmake.configure()
        return cmake

    def configure(self):
        tools.check_min_cppstd(self, "20")
        if os_info.is_linux:
            self.options["qt"].qtwayland = False
            self.options["qt"].with_x11 = True
            self.options["qt"].with_icu = True

        if self.settings.os == "Windows":
            self.options["qt"].with_freetype = False


    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def imports(self):
        if self.settings.os == "Windows":
            self.copy("Qt6Widgets*.dll", dst="bin", src="bin", keep_path=False)
            self.copy("Qt6Core*.dll", dst="bin", src="bin", keep_path=False)
            self.copy("Qt6Gui*.dll", dst="bin", src="bin", keep_path=False)
            self.copy("res/archdatadir/plugins/platforms/qwindows*.dll", dst="bin/platforms", src="", keep_path=False)
