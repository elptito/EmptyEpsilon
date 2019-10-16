const fs = require('fs');
const path = require('path');

process.on('uncaughtException', e => console.log(e.message, e.stack))
const defaultVarName = 'ScienceDatabase()';
let result = ``;

function main() {
    const rootPath = path.join(__dirname, 'Entries');
    handleDir(rootPath);
    saveResult();
}

/**
 * Goes over all the items in the dir and handles them recursively
 * @param {*} dirPath directory path
 * @param {*} _varName - DO NOT USE! if we have a directory inside a directory, we need to use a different 
 * variable name so that we don't override things by mistake. I just add '1' for every level
 */
function handleDir(dirPath, _varName = defaultVarName) {
    const items = fs.readdirSync(dirPath);
    for (const item of items) {
        const itemPath = path.join(dirPath, item);
        const stat = fs.statSync(itemPath);
        if (item === 'index.md') {
            const content = fs.readFileSync(itemPath);
            const [, metaData, data] = content.toString().split('---');
            handleMetaData(metaData, _varName);
            result += `${_varName}:setLongDescription([[\n${data.trim()}  \n]])\n`
        }
        else if (stat.isFile()) {
            handleFile(item, itemPath, _varName);
        } else {
            let newVarName = '';
            if (_varName === defaultVarName) {
                newVarName = 'space_objects';
                result += `${newVarName} = ScienceDatabase():setName('${item}')\n`;
            } else {
                newVarName = `${_varName}1`;
                result += `${newVarName} = ${_varName}:addEntry('${item}')\n`;
            }
            handleDir(itemPath, newVarName);
        }
    }
}

function handleFile(fileName, filePath, varName) {
    const cleanFileName = removeExtension(fileName);
    result += `item = ${varName}:addEntry('${cleanFileName}')\n`;
    const content = fs.readFileSync(filePath);
    const [, metaData, data] = content.toString().split('---');
    if(!data){
        console.log(`bad file structure`, filePath);
    } else {
        if (data.indexOf(String.fromCharCode(65533)) > -1) {
            console.log(`bad character ${data.indexOf(String.fromCharCode(65533))}`, filePath);
        }
        handleMetaData(metaData);
        result += `item:setLongDescription([[\n${data.trim()}  \n]])\n`
    }
}

function removeExtension(fileName) {
    return fileName.replace(/\.[^/.]+$/, "")
}

function saveResult() {
    fs.writeFileSync( path.join(__dirname, 'science_db.lua'), result);
}

function handleMetaData(metaData, varName = 'item') {
    const lines = metaData.split(/\r?\n/);
    for (const line of lines) {
        if (line.indexOf('password') > -1) {
            result += `${varName}:setPassword('${getValue(line)}')\n`;
        }
        if (line.indexOf('image') > -1) {
            result += `${varName}:setImage('db/${getValue(line)}')\n`;
        }
    }
}

function getValue(str) {
    return str.split(':')[1].trim();
}

console.log("exporting DB entries...");

main();

console.log("Done!");
