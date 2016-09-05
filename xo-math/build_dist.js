var fs = require('fs');

var g_IncludeRoot = '/include/';
var g_TemplateName = 'xo-math.h';
var g_TemplateText = null;
var g_IncludeNames = [
  'DetectSIMD.h',
  'Matrix4x4.h',
  'Matrix4x4Methods.h',
  'Matrix4x4Operators.h',
  'Quaternion.h',
  'QuaternionMethods.h',
  'QuaternionOperators.h',
  'SSE.h',
  'Vector2.h',
  'Vector2Methods.h',
  'Vector2Operators.h',
  'Vector3.h',
  'Vector3Methods.h',
  'Vector3Operators.h',
  'Vector4.h',
  'Vector4Methods.h',
  'Vector4Operators.h',
];
var g_IncludesText = [];
var g_DistName = '/../xo-math.h';

for(var i = 0; i < g_IncludeNames.length; ++i) {
  g_IncludesText[g_IncludeNames[i]] = null;
}

function SaveDistFile() {
  var output = '';
  var lines = g_TemplateText.split('\n');
  for(var i = 0; i < lines.length; ++i) {
    var includeFound = false;
    for(var j = 0; j < g_IncludeNames.length; ++j) {
      if(lines[i].indexOf('"' + g_IncludeNames[j] + '"') > 0) {
        output += g_IncludesText[g_IncludeNames[j]];
        includeFound = true;
        break;
      }
    }
    if(!includeFound) {
      output += lines[i] + '\n';
    }
  }

  fs.writeFile(__dirname + g_DistName, output, function(err) {
    if(err) {
      return console.error(err);
    }
    console.log(g_DistName, 'saved.');
  });
}


function TrySaveDistFile() {
if(!g_TemplateText)
    return;
  for(var i = 0; i < g_IncludeNames.length; ++i) {
    if(!g_IncludesText[g_IncludeNames[i]]) {
      return;
    }
  }
  SaveDistFile();
}

function IncludeLoaded(includeName, text) {
  g_IncludesText[includeName] = text+'\n';
  TrySaveDistFile();
}

function TemplateLoaded(text) {
  g_TemplateText = text;
  TrySaveDistFile();
}

function ReadInclude(name) {
  fs.readFile( __dirname + g_IncludeRoot + name, function (err, data) {
    if (err) {
      throw err;
    }
    IncludeLoaded(name, data.toString());
  });
}

for(var i = 0; i < g_IncludeNames.length; ++i) {
  ReadInclude(g_IncludeNames[i]);
}

fs.readFile( __dirname + g_IncludeRoot + g_TemplateName, function (err, data) {
  if (err) {
    throw err;
  }
  TemplateLoaded(data.toString());
});