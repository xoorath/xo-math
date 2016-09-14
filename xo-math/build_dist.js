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

String.prototype.replaceAll = function(search, replacement) {
  var target = this;
  return target.split(search).join(replacement);
};

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

    var split = data.toString().split('\n');
    var txt = '';

    var reading = false;
    for(var i = 0; i < split.length; ++i) {
      if(split[i].indexOf('//!<') > 0) {
        var subidx = split[i].indexOf('//!<');
        // take everything but the start of the doxygen  same-line comment
        txt += split[i].substring(0, subidx) + '\n';
      }
      else if(split[i].indexOf('//!') > 0) { 
        // do nothing, doxygen comment
      }
      else if(split[i].indexOf('//?') > 0) {
        // denotes a fake doxygen comment, to be excluded here and in doxygen
      }
      else if(split[i].indexOf('/*!') > 0) {
        // ignore doxygen block comment
        reading = false;
      }
      else if(split[i].indexOf('//>See') > 0) {
        var foundName = null;
        for(var j = i; j < split.length; ++j) {
          var nametag = '//! @name';
          var nameIdx = split[j].indexOf(nametag);
          if(nameIdx > 0) {
            foundName = split[j].substring(nameIdx+nametag.length);
            break;
          }
        }
        if(foundName != null) {
          foundName = foundName.trim();

          var initialWhitespace = split[i].substring(0, split[i].indexOf('//>See'));
          txt += initialWhitespace;
          txt += '////////////////////////////////////////////////////////////////////////// ' + foundName + '\n';

          foundName = foundName.toLowerCase().replaceAll(' ', '_').replaceAll('/', '').replaceAll('__', '_');

          txt += initialWhitespace
          txt += '// See: http://xo-math.rtfd.io/en/latest/classes/' + name.substring(0, name.indexOf('.h')).toLowerCase() + '.html#' + foundName + '\n';
        }
      }
      else if(reading) {
        txt += split[i] + '\n';
      }
      else {
        if(split[i].indexOf('XOMATH_INTERNAL') > 0) {
          reading = true;
          txt += split[i] + '\n';
        } else if (split[i].indexOf('*/') > 0) {
          reading = true;
        }
      }
    }
    IncludeLoaded(name, txt);
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