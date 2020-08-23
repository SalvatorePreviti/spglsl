#!/bin/bash -e

export SPGLSL_PROJECT_DIR="${PWD}"
if [ -d "${SPGLSL_PROJECT_DIR}/project/package.json" ] 
then
  SPGLSL_PROJECT_DIR="${SPGLSL_PROJECT_DIR}/project"
fi

export SPGLSL_ROOT_DIR="${SPGLSL_PROJECT_DIR}/.."

#
# install modules
#
cd $SPGLSL_PROJECT_DIR
if [ -d "./node_modules" ] 
then
  npm update
fi
if [ ! -d "./node_modules" ] 
then
  npm i
  npm update
fi

#
# install emscripten
#
if [ ! -d "${SPGLSL_ROOT_DIR}/emsdk" ] 
then
  cd $SPGLSL_ROOT_DIR
  git clone https://github.com/emscripten-core/emsdk.git
fi
cd $SPGLSL_ROOT_DIR/emsdk
if [ -d "$SPGLSL_ROOT_DIR/emsdk" ] 
then
  git config pull.rebase false
  git reset --hard
  git pull
  ./emsdk install latest-upstream
  ./emsdk activate latest-upstream
fi
cd $SPGLSL_PROJECT_DIR

#
# clone and update ANGLE
#
if [ ! -d "${SPGLSL_ROOT_DIR}/angle" ] 
then
  cd $SPGLSL_ROOT_DIR
  git clone git@github.com:google/angle.git
fi
cd $SPGLSL_ROOT_DIR/angle
if [ -d "${SPGLSL_ROOT_DIR}/angle" ] 
then
  git config pull.rebase false
  git reset --hard
  git pull
fi
cd $SPGLSL_PROJECT_DIR
