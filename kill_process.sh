#!/bin/bash

firefox &
t=$!
sleep 5
kill $t
