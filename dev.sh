docker build . -f ./dev.dockerfile -t nutspack-dev:latest

docker run -it -v `pwd`:/opt --rm nutspack-dev:latest /bin/bash