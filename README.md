# vicerp-server
Open source code vicerp-server. Server uses [VCMP Plugin SDK](https://forum.vc-mp.org/?topic=13.0).

## Build

### GNU/Linux

**Install redis server**
    apt-get install -y redis
    apt-get install -y libhiredis0.13

**Build and run**
    make
    make run

### Docker

**Build and run**
    ./docker-build.sh
    ./docker-run.sh

**Stop**
    ./docker-stop.sh

### docker-compose

**Build**
    docker-compose build

**Run**
    docker-compose up -d

**Stop**
    docker-compose down

### Dependencies

**Build**
* [Redis](https://redis.io/)
* [hiredis](https://redis.io/)
* [make](https://www.gnu.org/software/make/)

**Docker build**
* [Docker](https://www.docker.com/)
* [docker-compose](https://docs.docker.com/compose/)

## License
GNU General Public License (GPL), version 2