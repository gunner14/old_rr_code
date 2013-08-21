// const arrays
var prefixArr = ["large", "main", "head", "tiny", "xlarge"];
var widthArr = ["720", "200", "100", "50", "1024"];
var heightArr = ["720", "600", "300", "50", "1280"];
var qualityArr = ["88", "88", "88", "88", "88", "88"];
var cropArr = [0, 0, 0, 1, 0];

function findObj(theObj, theDoc)
{
  var p, i, foundObj;
  if(!theDoc) theDoc = document;
  if( (p = theObj.indexOf("?")) > 0 && parent.frames.length)
  { 
    theDoc = parent.frames[theObj.substring(p+1)].document;    
    theObj = theObj.substring(0,p); 
  } 
  if(!(foundObj = theDoc[theObj]) && theDoc.all) 
    foundObj = theDoc.all[theObj]; 

  for (i=0; !foundObj && i < theDoc.forms.length; i++)     
    foundObj = theDoc.forms[i][theObj]; 

  for(i=0; !foundObj && theDoc.layers && i < theDoc.layers.length; i++)     
    foundObj = findObj(theObj,theDoc.layers[i].document); 
    
  if(!foundObj && document.getElementById) 
    foundObj = document.getElementById(theObj);    
  return foundObj;
}

// add row
function AddRow(){ 
  var sel = findObj("imgTypeSel", document);
  var selIdx = sel.selectedIndex;
  if (selIdx < 0) {
    return alert("Select image size type first");
  }
  var imgTypeID = sel.options[selIdx].value;
  sel.options.remove(selIdx);
  
  var lastRowTxt = findObj("handleImgTBLastRow",document);
  var lastRow = parseInt(lastRowTxt.value);
  // head takes the 0th row
  var lastImgRow = lastRow - 1;
  var table = findObj("handleImgTB",document);
  var row = table.insertRow(table.rows.length);
  row.id = imgTypeID;
  // imgPrefix
  var colPrefix =row.insertCell(0);
  colPrefix.innerHTML = "<input name='imgprefix" + lastImgRow + "' id=imgprefix" + 
    lastImgRow + " type='text' value=" + prefixArr[imgTypeID] + " />";

  var colWidth = row.insertCell(1);
  colWidth.innerHTML = "<input name='imgwidth" + lastImgRow + "' id='imgwidth" + 
    lastImgRow + "' type='text' value=" + widthArr[imgTypeID] + " />";

  var colHeight = row.insertCell(2);
  colHeight.innerHTML = "<input name='imgheight" + lastImgRow + "' id='imgheight" + 
    lastImgRow + "' type='text' value=" + heightArr[imgTypeID] + " />";
    
  var colQuality = row.insertCell(3);
  colQuality.innerHTML = "<input name='imgquality" + lastImgRow + "' id='imgquality" 
    + lastImgRow + "' type='text' value=" + qualityArr[imgTypeID] + " />";
    
  var colCrop = row.insertCell(4);
  checked = "";
  if (cropArr[imgTypeID] == 1) {
    checked = " checked = 'checked'";
  }
  colCrop.innerHTML = "<input name='imgcrop" + lastImgRow + "' id='imgcrop" + lastImgRow +
    "' type='checkbox'" + checked + " />";
    
  var colPng = row.insertCell(5);
  colPng.innerHTML = "<input name='imgkeeppng" + lastImgRow + "' id='imgkeeppng" + 
    lastImgRow + "' type='checkbox'" + " />";
  
  var colGif = row.insertCell(6);
  colGif.innerHTML = "<input name='imgkeepgif" + lastImgRow + "' id='imgkeepgif" + 
    lastImgRow + "' type='checkbox'" + " />";
    
  var colSharpen = row.insertCell(7);
  colSharpen.innerHTML = "<input name='imgsharpen" + lastImgRow + "' id='imgsharpen"
    + lastImgRow + "' type='checkbox'" + " />";

  var colDelete = row.insertCell(8);
  colDelete.innerHTML = "<div align='center' style='width:40px'>" + 
    "<a href='javascript:;' onclick=\"DeleteRow('" + imgTypeID + "')\">删除</a>"
    + "</div>";

  lastRowTxt.value = (lastRow + 1).toString();
}

function DeleteRow(imgTypeID){
  var table = findObj("handleImgTB",document);
  var rowToDel = -1;
  for (var i=0;i<table.rows.length;i++) {
    if (table.rows[i].id == imgTypeID) {
      rowToDel = i;
      break;
    }
  }
  if (rowToDel == -1) {
    return;
  } else {
    // adjust element's name-value index
    for (var i = rowToDel + 1; i < table.rows.length; i++) {
      var row = table.rows[i];
      var imgRow = i - 2;
      for (var j = 0; j < row.cells.length - 1; j++) {
        row.cells[j].innerHTML = row.cells[j].innerHTML.replace(/([a-z]+)(\d+)/g,
        "$1" + imgRow);
      }
    }
    // delete row
    table.deleteRow(rowToDel);
    // decrease lastRow
    var lastRowTxt = findObj("handleImgTBLastRow",document);
    var lastRow = parseInt(lastRowTxt.value);
    lastRowTxt.value = (lastRow - 1).toString();
  }
  var sel = findObj("imgTypeSel", document);
  var item = new Option(prefixArr[imgTypeID], imgTypeID);
  sel.options.add(item);
}
