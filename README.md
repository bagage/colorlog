# colorlog

[![Build Status](https://travis-ci.org/bagage/colorlog.svg?branch=master)](https://travis-ci.org/bagage/colorlog)

This colorize logs/output by finding regular patterns.

This is a fork of [color](https://github.com/mateuszlis/color), an open source program that allows to colorize console
output eg. from compilation log, syslog or any other log displayed on UNIX
console.
It allows to define your own set of coloring rules, but comes with predefined
ready to used schemes for `gcc` output, `syslog`, `apachelog`, etc.
To see more, check `/etc/colorlog.conf` for information about defined schemes.

## How to use colorlog

### Usage

Either one of the following:

```sh
[some command] | colorlog [colorlog-scheme]
colorlog [some command]
```

### Examples

```
$ cat /var/log/syslog | colorlog syslog
# or
$ colorlog syslog cat /var/log/syslog

$ cat /var/log/messages/ | colorlog

$ make 2>&1 | colorlog
#or, identically
$ colorlog make
```

## License

GPLv3

## Why forking and renaming it

`color` was unmaintained for 3 years and I wished to add some extra features.
I renamed it to `colorlog` to make it easier to find (`color` being a generic
name referencing many things and not only logs highlighting software).
