# vicerp-server
Open source code vicerp-server. Server uses [VCMP Plugin SDK](https://forum.vc-mp.org/?topic=13.0).

## Build

```console
apt-get install -y libhiredis0.13 libmysqlclient-dev
make build
make install
docker-compose build
docker-compose up -d
````

### Rebuild and restart server
```console
make all
````

## Dependencies

* libhiredis0.13
* libmysqlclient-dev
* [make](https://www.gnu.org/software/make/)
* [Docker](https://www.docker.com/)
* [docker-compose](https://docs.docker.com/compose/)

## Tested
* Ubuntu 18.04 LTS

## License
GNU General Public License (GPL), version 2