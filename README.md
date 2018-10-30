# 34C3 Junior CTF pwnables

Source code and exploits for the 34C3 Junior CTF pwnables.

To recreate the CTF environment with docker:

```
./docker_build.sh <module_name>
./docker_run.sh <module_name>
```
module_names: {billboard, giftwrapper, giftwrapper2, mate, mate2, shredder}


Connect to the service:
```
nc localhost 12345
```

For each challenge the files in the respective subdirectory were given.


Use the following command to retrieve the libc used in the docker environment:
```
docker cp module_name:/lib/x86_64-linux-gnu/libc.so.6 .
```


## Digital Billboard

We bought a new Digital Billboard for CTF advertisement.


## Gift Wrapping Factory

Have trouble wrapping your gifts nicely? Take a look at this new service.


## Gift Wrapping Factory 2.0

Wrapping gifts is now even more fun!


## Mate Bottling Plant Control Center

To guarantee a constant supply of Mate we built our own Mate Bottling Plant.


## Mate Bottling Plant Control Center 2.0

There were some minor problems with the previous version of Mate Bottling Plant. We hired the world's best Mate specialists to improve usability, functionality and security in our new Mate Bottling Plant 2.0.


## Gift Shredder

As a new service¹ for our customers who are not satisfied with this year's christmas gifts, we launched the modern, state of the art Gift Shredder Plant.

(¹) Unfortunately we are not able to compensate for your financial loss.
