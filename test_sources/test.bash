# SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
#
# SPDX-License-Identifier: MIT

# to compile the code:
# build and run the requite C++ compiler with your requite source files in the build directory.

# to link the application (requires clang) (can use GCC instead if you want)
clang <insert paths to built obj files here> -o example -lm

# to give permissions to execute the compiled binary on linux:
chmod +x example

# to run the application:
./example

# to print the return code:
echo $?