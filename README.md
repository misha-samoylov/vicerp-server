# vicerp-server
Open source code vicerp-server. Server uses [VCMP Plugin SDK](https://forum.vc-mp.org/?topic=13.0).

## Build

### GNU/Linux

**Install redis server**
```console
apt-get install -y redis  
apt-get install -y libhiredis0.13
````

Edit file `config.h` define `REDIS_SERVER` to address redis server. For example:

````c
#define REDIS_SERVER "127.0.0.1"
````

**Build and run**
```console
make
make run
````

### docker-compose  
```console
docker-compose build
docker-compose up -d
````

### Dependencies

**Native build**
* [Redis](https://redis.io/)
* [hiredis](https://redis.io/)
* [make](https://www.gnu.org/software/make/)

**Docker build**
* [Docker](https://www.docker.com/)
* [docker-compose](https://docs.docker.com/compose/)

## License
GNU General Public License (GPL), version 2