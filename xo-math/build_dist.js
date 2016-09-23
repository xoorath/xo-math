var fs = require('fs');

var g_IncludeRoot = '/include/';
var g_SourceRoot = '/src/';

var g_IncludeInputName = 'xo-math.h';
var g_IncludeInputText = null;

var g_SourceInputName = 'xo-math.cpp';
var g_SourceInputText = null;

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
for(var i = 0; i < g_IncludeNames.length; ++i) {
  g_IncludesText[g_IncludeNames[i]] = null;
}

// these are copied directly to the destination directory without any processing.
var g_StraightCopyIncludes = [
  'xo-math-config.h'
];

var g_SourcesNames = [
  'Matrix4x4.cpp',
  'Quaternion.cpp',
  'SSE.cpp',
  'Vector2.cpp',
  'Vector3.cpp',
  'Vector4.cpp'
];
var g_SourcesText = [];
for(var i = 0; i < g_SourcesNames.length; ++i) {
  g_SourcesText[g_SourcesNames[i]] = null;
}

var g_OutPath = '/../';
var g_IncludeOutName = 'xo-math.h';
var g_SourceOutName = 'xo-math.cpp';

String.prototype.replaceAll = function(search, replacement) {
  var target = this;
  return target.split(search).join(replacement);
};

function SaveIncludeOutFile() {
  var output = '';
  var lines = g_IncludeInputText.split('\n');
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

  fs.writeFile(__dirname + g_OutPath + g_IncludeOutName, output, function(err) {
    if(err) {
      return console.error(err);
    }
    console.log(g_IncludeOutName, 'saved.');
  });
}

function SaveSourceOutFile() {
  var output = '';
  var lines = g_SourceInputText.split('\n');
  
  for(var i = 0; i < lines.length; ++i) {
    var includeFound = false;
    // for(var j = 0; j < g_IncludeNames.length; ++j) {
    //   if(lines[i].indexOf('"' + g_IncludeNames[j] + '"') > 0) {
    //     output += g_IncludesText[g_IncludeNames[j]];
    //     includeFound = true;
    //     break;
    //   }
    // }

    if(lines[i].indexOf('XOMATH_END_XO_NS') >= 0) {
      for(var name in g_SourcesText) {
        output += g_SourcesText[name];
      }
    }

    if(!includeFound) {
      output += lines[i] + '\n';
    }
  }

  fs.writeFile(__dirname + g_OutPath + g_SourceOutName, output, function(err) {
    if(err) {
      return console.error(err);
    }
    console.log(g_SourceOutName, 'saved.');
  });
}

function TrySaveIncludeOutFile() {
  if(!g_IncludeInputText)
    return;
  for(var i = 0; i < g_IncludeNames.length; ++i) {
    if(!g_IncludesText[g_IncludeNames[i]]) {
      return;
    }
  }
  SaveIncludeOutFile();
}

function TrySaveSourceOutFile() {
  if(!g_SourceInputText)
    return;
  for(var i = 0; i < g_SourcesNames.length; ++i) {
    if(!g_SourcesText[g_SourcesNames[i]]) {
      return;
    }
  }
  SaveSourceOutFile();
}

function IncludeLoaded(includeName, text) {
  g_IncludesText[includeName] = text+'\n';
  TrySaveIncludeOutFile();
}

function SourceLoaded(sourceName, text) {
  g_SourcesText[sourceName] = text+'\n';
  TrySaveSourceOutFile();
}

function IncludeTemplateLoaded(text) {
  g_IncludeInputText = text;
  TrySaveIncludeOutFile();
}

function SourceTemplateLoaded(text) {
  g_SourceInputText = text;
  TrySaveSourceOutFile();
}

function ReadInclude(name) {
  fs.readFile( __dirname + g_IncludeRoot + name, function (err, data) {
    if (err) {
      throw err;
    }

    var split = data.toString().split('\n');
    var txt = '';

    var reading = false;
    var optionallyRemoveNextEmptyLine = false;
    for(var i = 0; i < split.length; ++i) {
      if(split[i].indexOf('//!<') >= 0) {
        var subidx = split[i].indexOf('//!<');
        // take everything but the start of the doxygen  same-line comment
        txt += split[i].substring(0, subidx) + '\n';
      }
      else if(split[i].indexOf('//!') >= 0) { 
        // do nothing, doxygen comment
      }
      else if(split[i].indexOf('//?') >= 0) {
        // denotes a fake doxygen comment, to be excluded here and in doxygen
      }
      else if(split[i].indexOf('/*!') >= 0) {
        // ignore doxygen block comment
        reading = false;
      }
      else if(split[i].indexOf('//>See') >= 0) {
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
          optionallyRemoveNextEmptyLine = true;
        }
      }
      else if(reading) {
        var skipLine = false

        if(optionallyRemoveNextEmptyLine) {
          if(split[i].trim().length === 0)
            skipLine = true;
          optionallyRemoveNextEmptyLine = false;
        }

        if(!skipLine)
          txt += split[i] + '\n';
      }
      else {
        if(split[i].indexOf('XOMATH_INTERNAL') >= 0) {
          reading = true;
          txt += split[i] + '\n';
        } else if (split[i].indexOf('*/') >= 0) {
          reading = true;
        }
      }
    }
    IncludeLoaded(name, txt);
  });
}

function ReadSource(name) {
  fs.readFile( __dirname + g_SourceRoot + name, function (err, data) {
    if (err) {
      throw err;
    }

    var split = data.toString().split('\n');
    var txt = '';

    var reading = false;

    for(var i = 0; i < split.length; ++i) {
      if(split[i].indexOf('XOMATH_BEGIN_XO_NS') >= 0) {
        reading = true;
        txt += '////////////////////////////////////////////////////////////////////////// ' + name + '\n';
      }
      else if(reading) {
        if(split[i].indexOf('XOMATH_END_XO_NS') >= 0) {
          reading = false;
          break;
        }
        txt += split[i] + '\n';
      }
    }
    SourceLoaded(name, txt);
  });
}

for(var i = 0; i < g_IncludeNames.length; ++i) {
  ReadInclude(g_IncludeNames[i]);
}

for(var i = 0; i < g_SourcesNames.length; ++i) {
  ReadSource(g_SourcesNames[i]);
}

fs.readFile( __dirname + g_IncludeRoot + g_IncludeInputName, function (err, data) {
  if (err) {
    throw err;
  }
  IncludeTemplateLoaded(data.toString());
});

fs.readFile( __dirname + g_SourceRoot + g_SourceInputName, function (err, data) {
  if (err) {
    throw err;
  }
  SourceTemplateLoaded(data.toString());
});

// http://stackoverflow.com/a/14387791
function copyFile(source, target, cb) {
  var cbCalled = false;

  var rd = fs.createReadStream(source);
  rd.on("error", function(err) {
    done(err);
  });
  var wr = fs.createWriteStream(target);
  wr.on("error", function(err) {
    done(err);
  });
  wr.on("close", function(ex) {
    done();
  });
  rd.pipe(wr);

  function done(err) {
    if (!cbCalled) {
      cb(err);
      cbCalled = true;
    }
  }
}

for(var i = 0; i < g_StraightCopyIncludes.length; ++i) {
  copyFile(
    __dirname + g_IncludeRoot + g_StraightCopyIncludes[i], 
    __dirname + g_OutPath + g_StraightCopyIncludes[i], 
    function(err) {
      if(err) return console.error(err);
      console.log('file copied.');
    });
}