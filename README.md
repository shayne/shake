[![Build Status](https://travis-ci.org/shayne/shake.svg?branch=master)](https://travis-ci.org/shayne/shake)

# shake

Shake was created to replace make for cases where it was being misused as a simple task runner, after growing frustrated trying to use Makefiles to adapt seemingly innocuous behaviors. Implementing script-wide environment variables or passing additional command-line arguments through to tasks was less than straightforward - or even just calling make from a directory other than the project's root. 

What I wanted was to make make behave like bash, allowing me to do everything I wanted using techniques I already knew. The idea to replace make with something more shell like is **shake**.

Shake reads from a ``Shakefile`` which can contain any scripting you'd like to help automate your tasks. Tasks can be defined within the Shakefile itself, as Bash functions, or as standalone scripts, written in whatever language works for you.

# Installing

Getting started is easy. You can download the latest version from our [Release](https://github.com/shayne/shake/releases) page, or use one of the following automated methods:

**curl**

```
$ curl https://shakeit.co | sh
```

**homebrew**

```
$ brew install shayne/shake/shake
```

# Shakefile

Shake can create a Shakefile for you, providing a bit of a tutorial and generating a couple examples at the same time.

```
$ cd /path/to/my-project
$ shake --init

This utility will walk you through the basic shake setup process.
It only covers the required configuration, and suggests typical defaults.

Press CTRL-C at any time to quit. No changes are made until you've finished.

#1) Choose a project root
    This directory will the top-most directory in your project
    that will recognize your shake commands.

TIP: You can run shake from any sub-directory of your project
root dir: /home/jdoe/my-project

...
```

# More

Ideally everyone would use shake, but that's not realistic. Often projects become littered with scripts, some of which intended to run, some to be sourced, some not to be touched. It becomes messy and difficult to find or remember the script you need.

Shake task scripts are all placed in a specified directory (e.g. `scripts/`), all with the same prefix (e.g. `run-`, `scripts/run-dev-server`). This provides built-in organization and discoverability and the ability for scripts to be run with or without using shake.
