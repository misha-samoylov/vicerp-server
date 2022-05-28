FROM ubuntu:18.04

# update packages
RUN apt-get update -y && apt-get upgrade -y

# install libraries
RUN apt-get install -y libhiredis0.13 libmysqlclient-dev

# create a user group 'xyzgroup'
RUN groupadd --gid 2000 xyzgroup

# create a user 'appuser' under 'xyzgroup'
RUN useradd --uid 2000 --gid xyzgroup --shell /bin/bash --create-home appuser

# switch to 'appuser'
USER appuser

# copy files
WORKDIR /home/appuser
RUN mkdir plugins
COPY ./src/mpsvrrel64 .

# open mapper port
EXPOSE 8192

CMD ["./mpsvrrel64"]
