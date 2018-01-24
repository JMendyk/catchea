#!/usr/bin/env bash

# This script downloads dependencies,
# performs required configuration/compilation/purging
# and organises them.

set -e
cd $(dirname `readlink -f "$0"`)
cd ../deps


HISTFILE=`pwd`/deps_update_history.txt

# Change datetime's locale to get month/weekday names in english
LC_TIME_copy=$LC_TIME
LC_TIME=en_US.UTF-8
HEADER="Updating dependencies - $(date "+%a %b %d %T %Y %z")"
LC_TIME=$LC_TIME_copy


function log_dependency {
    info=`git log --pretty=format:"%h, %ad : %s" | head -n 1`
    echo "Dependency: $1" >> $HISTFILE
    echo $info >> $HISTFILE
}

function log_dependency_file {
    info=`git log --pretty=format:"%h, %ad : %s" --follow $2 | head -n 1`
    echo "Dependency: $1" >> $HISTFILE
    echo $info >> $HISTFILE
}

rm -rf gl3w gl3w_repo \
       glfw glfw_repo \
       imgui imgui_repo \
       imgui_impl_glfw_gl3 \
       stb_repo stb_image stb_image_write

echo $HEADER >> $HISTFILE



##### GL3W #####
mkdir gl3w
git clone https://github.com/skaslev/gl3w.git gl3w_repo
cd gl3w_repo
    git checkout master
    log_dependency gl3w
    cmake . && make
    cp -r include/* ../gl3w
    cp src/gl3w.c ../gl3w
cd ..
rm -rf gl3w_repo



##### GLFW #####
git clone https://github.com/glfw/glfw.git glfw_repo
cd glfw_repo
    git checkout 3.2.1
    log_dependency glfw
    rm -rf .git .gitignore .github docs examples tests .appveyor.yml .travis.yml
cd ..
mv glfw_repo glfw



##### ImGUI #####
mkdir imgui_impl_glfw_gl3
git clone https://github.com/Flix01/imgui.git imgui_repo
cd imgui_repo
    # git checkout 2015-10-Addons
    log_dependency "imgui + addons"
    cp examples/opengl3_example/imgui_impl_glfw_gl3.h \
       examples/opengl3_example/imgui_impl_glfw_gl3.cpp \
       ../imgui_impl_glfw_gl3/
    rm -rf .git .gitignore examples .travis.yml TODO.txt
cd ..
mv imgui_repo imgui



##### stb_image #####
mkdir stb_image
mkdir stb_image_write
git clone https://github.com/nothings/stb stb_repo
cd stb_repo
    git checkout master
    log_dependency_file stb_image stb_image.h
    log_dependency_file stb_image stb_image_write.h
    cp stb_image.h ../stb_image/
    cp stb_image_write.h ../stb_image_write/
cd ..
rm -rf stb_repo



echo $HEADER | sed 's/./-/g' >> $HISTFILE
echo "" >> $HISTFILE
