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
        if (!fs.existsSync(combined)) {
            fs.mkdirSync(combined);
        }
    }
}

function CopyLibs(destination, libs) {
    mkdirRecursive(destination);
    for(var i = 0; i < libs.length; ++i) {
        libs[i] =  path.resolve(__dirname, libs[i]).replace(/\\/g, '/');
        if(fs.existsSync(libs[i]) === false) {
            console.error(`library doesn't exist: ${libs[i]}`);
            libs = libs.splice(i, 1);
            --i;
        }
    }
    for (let i = 0; i < libs.length; ++i) {
        var nameIdx = libs[i].lastIndexOf('/');
        var name = libs[i].substr(nameIdx);
        var filePath = libs[i];
        var dest =  destination + '/' + name;
        fs.copyFile(filePath, dest, (err) => { if(err) console.error(err); });
    }
}

CopyLibs(path.resolve(__dirname, `../${genieParams[0]}/x32`).replace(/\\/g, '/'), dyLibs32);
CopyLibs(path.resolve(__dirname, `../${genieParams[0]}/x64`).replace(/\\/g, '/'), dyLibs64);

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