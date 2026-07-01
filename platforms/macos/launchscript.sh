#!/bin/sh
# We want a universal binary like system, but
# we want to run the i386 slice on x86_64 Tiger.

# This has to be a shell script because a C program
# cannot reliably get the path of the executable,
# and we need to run other binaries at a relative location
# from this script.

[ "${0%/*}" = "$0" ] && scriptroot="." || scriptroot="${0%/*}"
cd "$scriptroot" || exit 1

[ "$(arch)" = "ppc" ] &&
    exec ./libexec/nbcraft-powerpc "$@"

arch="$(./libexec/arch)"

if [ "$arch" = "x86_64" ]; then
    case $(uname -r) in
        (8.*|9.*|10.*)
            # Tiger, Leopard, or Snow Leopard
            exec ./libexec/nbcraft-i386 "$@"
        ;;
        (*)
            if [ "$(sysctl -n sysctl.proc_translated 2>/dev/null)" = "1" ]; then
                # i hate rosetta
                exec ./libexec/nbcraft-arm64 "$@"
            else
                exec ./libexec/nbcraft-x86_64 "$@"
            fi
        ;;
    esac
fi

exec "./libexec/nbcraft-$arch" "$@"
