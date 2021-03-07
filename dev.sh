docker build . -f ./dev.dockerfile -t cyw0ng/nutspack-dev:latest

docker run -it -v `pwd`:/opt --rm cyw0ng/nutspack-dev:latest /bin/bash