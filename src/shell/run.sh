#! /bin/bash

source ./src/shell/progress.sh || exit 1

start=0
end=15
for ii in $(seq $start $end); do
    sleep 0.1
    progressbar "Installing Eve" $ii $end
done
echo