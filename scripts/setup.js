"use strict";

const { spawn } = require('child_process');
const path = require('path');

let genieParams = process.argv.splice(2);

if(genieParams.length === 0) {
	const defaultGenieParam = 'vs2017';
	console.warn(`No params passed to genie. Defaulting to ${defaultGenieParam}`);
	genieParams.push(defaultGenieParam);
} else {
	console.log(`starting genie.exe with arguments: ${genieParams}`);
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