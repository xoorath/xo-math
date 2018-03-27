"use strict";

const { spawn } = require('child_process');
const path = require('path');
const fs = require('fs');

let genieParams = process.argv.splice(2);

if (genieParams.length === 0) {
    const defaultGenieParam = 'vs2017';
    console.warn(`No params passed to genie. Defaulting to ${defaultGenieParam}`);
    genieParams.push(defaultGenieParam);
} else {
    console.log(`starting genie.exe with arguments: ${genieParams}`);
}

let dyLibs64 = [
    '../demos/glew-2.1.0/bin/Release/x64/glew32.dll',
    '../demos/SDL2-2.0.8/lib/x64/sdl2.dll'
];

let dyLibs32 = [
    '../demos/glew-2.1.0/bin/Release/Win32/glew32.dll',
    '../demos/SDL2-2.0.8/lib/x86/sdl2.dll'
];

function mkdirRecursive(dirName) {
    var split = dirName.split('/');
    var combined = split[0] + '/';

    for (var i = 1; i < split.length; ++i) {
        combined += split[i] + '/';
        console.log('making dir: ' + combined);
        if (!fs.existsSync(combined)) {
            console.log('done');
            fs.mkdirSync(combined);
        } else {
            console.log('it exists');
        }
    }
}

var libDestination = path.resolve(__dirname, `../${genieParams[0]}/x32`);
console.log(`libDestination ${libDestination}`);
libDestination = libDestination.replace(/\\/g, '/');
console.log(`libDestination ${libDestination}`);
mkdirRecursive(libDestination);
for (let i = 0; i < dyLibs32.length; ++i) {
    var name = dyLibs32[i].substr(0, dyLibs32[i].lastIndexOf('/'));
    var filePath = path.resolve(__dirname, dyLibs32[i]);
    fs.copyFile(filePath, libDestination + '/' + name, (err) => { console.error(err); });
}

libDestination = path.resolve(__dirname, `../${genieParams[0]}/x64`);
console.log(`libDestination ${libDestination}`);
libDestination = libDestination.replace(/\\/g, '/');
console.log(`libDestination ${libDestination}`);
mkdirRecursive(libDestination);
for (let i = 0; i < dyLibs64.length; ++i) {
    var name = dyLibs64[i].substr(0, dyLibs64[i].lastIndexOf('\\'));
    var filePath = path.resolve(__dirname, dyLibs64[i]);
    fs.copyFile(filePath, libDestination + '/' + name, (err) => { console.error(err); });
}

const genie = spawn(
    path.resolve(__dirname, '../bin/genie.exe'),
    genieParams,
    {
        shell: true,
        encoding: 'utf8'
    });

genie.stdout.on('data',
    /** @param {Buffer} data*/
    (data) => {
        console.log(typeof data == 'object' ? data.toString() : data);
    });

genie.stderr.on('data',
    /** @param {Buffer} data*/
    (data) => {
        console.error(typeof data == 'object' ? data.toString() : data);
    });

genie.on('exit',
    /** @param {number} code */
    (code) => {
        console.log(`Genie exited with code ${code}.`);
    });