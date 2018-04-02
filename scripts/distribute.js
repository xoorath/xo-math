"use strict";

const path = require('path');
const fs = require('fs');

const xomathRelativeFilePath = '../xo-math/xo-math.h';
const xomathFilePath = path.resolve(__dirname, xomathRelativeFilePath);
const xomathSourceDirectory = path.resolve(__dirname, '../source/');
const xomathSourceFilePath = path.resolve(xomathSourceDirectory, 'xo-math.h');

function RemoveTrailingWhitespace(str) {
    return str.replace(/^\s+|\s+$/g, "");
}

function ReplaceTabsWithSpaces(str) {
    return str.replace(/\t/g, '    ');
}

/** 
 * @param {string|Error} err
*/
function ErrorHandler(err) {
    if(typeof err == "string")
        console.error(err);
    else
        console.error(`Error: ${err.message}${err.stack ?'\n'+err.stack : ''}`);
}

/** 
 * @returns {Promise<bool>}
*/
function DeleteSingleFile(filename) { return new Promise((resolve, reject) => {
    fs.exists(filename, (exists) => {
        if(exists === false) return resolve(false);
        fs.unlink(filename, (err) => {
            if(err) return reject(err);
            return resolve(true);
        });
    });
});}

/** 
 * @returns {Promise}
*/
function ClearOldDistribution() { return new Promise((resolve, reject) => {
    console.log(`Deleting ${xomathRelativeFilePath}`);
    resolve(DeleteSingleFile(xomathFilePath).then(()=>{console.log(`Deleted ${xomathRelativeFilePath}`);}));
});}

/**
 * 
 * @param {string} filePath 
 * @param {string} text 
 * @returns {Promise}
 */
function PrintStringToFile(filePath, text) { return new Promise((resolve, reject) => {
    fs.writeFile(filePath, text, (err) => {
        if(err) return reject(err);
        resolve();
    });
});}

/**
 * 
 * @param {string} filePath 
 * @returns {Promise<string>}
 */
function ReadFileContents(filePath) { return new Promise((resolve, reject) => {
    console.log(`Reading file: ${filePath}`);
    fs.readFile(filePath, (err, data) => {
        if(err) return reject(err);
        console.log(`File read`);
        resolve(data.toString('utf8'));
    });
});}

/**
 * @param {string} filePath
 * @param {string|Promise<string>} contentStringOrPromise 
 * @returns {Promise<void>}
 */
function PrintToFile(filePath, contentStringOrPromise) { return new Promise((resolve, reject) => {
    console.log(`Writing contents to ${filePath}`);
    if(typeof contentStringOrPromise === "string")
    {
        resolve(PrintStringToFile(filePath, contentStringOrPromise).then(()=>{
            console.log('Contents written to file.');
        }));
    }
    else
    {
        contentStringOrPromise.then((text) => {
            resolve(PrintStringToFile(filePath, text).then(()=>{
                console.log('Contents written to file.');
            }));
        }).catch(reject);
    }
});}


/**
 * @typedef SourceMap
 * @prop {String} fileName File contents keyed by the file name.
 */

/**
 * @param {string} directory 
 * @returns {Promise<SourceMap>}
 */
function BuildSourceMap(directory) { return new Promise((resolve, reject) => {
    console.log('Building source map.');
    fs.readdir(directory, (err, files) => {
        if(err) return reject(err);
        var result = {};
        for(var i = 0; i < files.length; ++i) {
            if(files[i].endsWith('.h') && !files[i].endsWith('xo-math.h')) {
                result[files[i]] = '';
            }
        }
        /** @type {Promise<string>[]} */
        var awaitingPromises = [];
        for(let key in result) {
            let fullPath = path.resolve(xomathSourceDirectory, key);
            awaitingPromises.push(ReadFileContents(fullPath)
            .then(contents => {
                var substrIdx = key.indexOf('/');
                if(substrIdx < 0) {
                    substrIdx = key.indexOf('\\');
                    if(substrIdx < 0) substrIdx = 0;
                }
                const inlineBeginKeyword = '$inline_begin';
                var startingLineNumber = 0;
                var inlineBeginIdx = contents.indexOf(inlineBeginKeyword);
                if(inlineBeginIdx > 0)
                {
                    startingLineNumber = (contents.substr(0, inlineBeginIdx).match(/\r?\n/g) || '').length + 1;
                    contents = contents.substr(contents.indexOf('\n', inlineBeginIdx)+1); // +1 to get rid of the newline after the keyword.
                }
                var simpleKey = key.substring(substrIdx);
                contents = 
`////////////////////////////////////////////////////////////////////////////////////////// ${simpleKey} inlined
#line ${startingLineNumber+1} "${simpleKey}"
${contents}
////////////////////////////////////////////////////////////////////////////////////////// end ${simpleKey} inline`;
                result[simpleKey] = contents;
            }));
        }
        Promise.all(awaitingPromises)
            .then(values => {
                console.log('Source map built.');
                // ignore the values variable, it just contains file contents. We want the map we're building in result
                resolve(result);
            })
            .catch(reject);
    });
});}

function BuildNewDistribution() { return new Promise((resolve, reject) => {
    console.log('Building distribution file.');
    BuildSourceMap(xomathSourceDirectory)
    .then(xoSourceMapContent => {
        ReadFileContents(xomathSourceFilePath).then(xoSourceContent => {
            for(var key in xoSourceMapContent) {
                var keyInclude = `#include "${key}"`;
                console.log(`inlining ${keyInclude} in xo-math.h`);
                let idx = xoSourceContent.indexOf(keyInclude);
                while(idx >= 0) {
                    xoSourceContent = xoSourceContent.replace(keyInclude, xoSourceMapContent[key])
                    idx = xoSourceContent.indexOf(keyInclude);
                }
            }
            xoSourceContent = ReplaceTabsWithSpaces(xoSourceContent);
            xoSourceContent = RemoveTrailingWhitespace(xoSourceContent);
            resolve(PrintToFile(xomathFilePath, xoSourceContent).then(()=>{
                console.log('Distribution file built.');
            }));
        }).catch(reject);
    }).catch(reject);
});}

ClearOldDistribution().then(BuildNewDistribution).catch(ErrorHandler);