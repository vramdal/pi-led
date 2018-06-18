./copy-from-mac.sh
pushd tmp/pi-led
# TODO: --nodedir=$path, https://github.com/nodejs/node-gyp
yarn install --unsafe-perm --fallback-to-build
popd