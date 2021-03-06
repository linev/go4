(function(){

   if (typeof JSROOT != "object") {
      var e1 = new Error("condition.js requires JSROOT to be already loaded");
      e1.source = "condition.js";
      throw e1;
   }

   if (typeof GO4 != "object") {
      var e1 = new Error("condition.js requires GO4 to be already loaded");
      e1.source = "condition.js";
      throw e1;
   }

   // =========================================================================================

   let BasePainter = JSROOT.BasePainter || JSROOT.TBasePainter;

   GO4.ConditionEditor = function(divid, cond) {
      BasePainter.call(this, divid);
      if (this.SetDivId) this.SetDivId(divid);
      this.cond = cond;
      this.changes = ["dummy", "init"];
      this.ClearChanges();
   }

   GO4.ConditionEditor.prototype = Object.create(BasePainter.prototype);

   GO4.ConditionEditor.prototype.isPolyCond = function() {
       return ((this.cond._typename == "TGo4PolyCond") || (this.cond._typename == "TGo4ShapedCond"));
   }

   GO4.ConditionEditor.prototype.isEllipseCond = function() {
       return (this.cond._typename == "TGo4ShapedCond");
   }

   // add identifier of changed element to list, make warning sign visible
   GO4.ConditionEditor.prototype.MarkChanged = function(key) {
      // first avoid duplicate keys:
      for (var index = 0; index < this.changes.length; index++) {
         if (this.changes[index]== key) return;
      }
      this.changes.push(key);
      console.log("Mark changed :%s", key);
      var id = "#" + this.get_main_id();

      $(id+" .buttonChangeLabel").show();// show warning sign
   }

   // clear changed elements' ist, make warning sign invisible
   GO4.ConditionEditor.prototype.ClearChanges = function() {
      for (var index = 0; index < this.changes.length ; index++) {
         var removed=this.changes.pop();
         console.log("Clear changes removed :%s", removed);
      }
      var id = this.get_main_id();
      if (id) $("#" + id + " .buttonChangeLabel").hide(); // hide warning sign
   }

   // scan changed value list and return optionstring to be send to server
   GO4.ConditionEditor.prototype.EvaluateChanges = function(optionstring) {
      var id = "#" + this.get_main_id();
      var index;
      var len=this.changes.length;
      for (index = 0; index < len ; index++) {
         //var cursor=changes.pop();
         var key=this.changes[index];
         //console.log("Evaluate change key:%s", key);

         // here mapping of key to editor field:
         if(key=="limits")
         {
            var xmin=$(id+" .cond_xmin")[0].value;
            var xmax=$(id+" .cond_xmax")[0].value;
            optionstring +="&xmin="+xmin+"&xmax="+xmax;
            this.cond.fLow1 = xmin;
            this.cond.fUp1 = xmax;
            if (this.cond.fiDim==2) {
               var ymin=$(id+" .cond_ymin")[0].value;
               var ymax=$(id+" .cond_ymax")[0].value;
               this.cond.fLow2 = xmin;
               this.cond.fUp2 = xmax;
               optionstring +="&ymin="+ymin+"&ymax="+ymax;
            }
         }
         else if(key=="polygon")
         {
            var npoints=$(id+" .cut_points")[0].value;
            optionstring +="&npolygon="+npoints;
            // TODO: set display of polygon points
            for(i=0; i<npoints; ++i)
            {
               var x = $(id + " .cut_values input").eq(2*i)[0].value;
               var y = $(id + " .cut_values input").eq(2*i+1)[0].value;
               optionstring +="&x"+i+"="+x+"&y"+i+"="+y;
            }
         }
         else if (key=="ellinpts"){
            var val=$(id+" .cond_ellipse_points")[0].value;
            optionstring +="&"+key+"="+val;
         }
         else if (key=="ellicx"){
            var val=$(id+" .cond_ellipse_cx")[0].value;
            optionstring +="&"+key+"="+val;
         }
         else if (key=="ellicy"){
            var val=$(id+" .cond_ellipse_cy")[0].value;
            optionstring +="&"+key+"="+val;
         }
         else if (key=="ellia1"){
            var val=$(id+" .cond_ellipse_a1")[0].value;
            optionstring +="&"+key+"="+val;
         }
         else if (key=="ellia2"){
            var val=$(id+" .cond_ellipse_a2")[0].value;
            optionstring +="&"+key+"="+val;
         }
         else if (key=="ellishape"){
            var arg=$(id+" .cond_ellipse_iscircle")[0].value;
            optionstring +="&"+key+"="+arg;
         }
         else if (key=="ellith"){
            var val=$(id+" .cond_ellipse_theta")[0].value;
            optionstring +="&"+key+"="+val;
         }
         else if (key=="resultmode"){
            var selected=$(id+" .cond_execmode")[0].value;
            optionstring +="&"+key+"="+selected;
         }
         else if (key=="invertmode"){
            var selected=$(id+" .cond_invertmode")[0].value;
            optionstring +="&"+key+"="+selected;
         }
         else if (key=="visible"){
            var checked=$(id+" .cond_visible")[0].checked;
            var arg= (checked ? "1" : "0");
            optionstring +="&"+key+"="+arg;
         }
         else if (key=="labeldraw"){
            var checked=$(id+" .cond_label")[0].checked;
            var arg= (checked ? "1" : "0");
            this.cond.fbLabelDraw=arg;
            optionstring +="&"+key+"="+arg;
         }
         else if (key=="limitsdraw"){
            var checked=$(id+" .cond_limits")[0].checked;
            var arg= (checked ? "1" : "0");
            this.cond.fbLimitsDraw=arg;
            optionstring +="&"+key+"="+arg;
         }
         else if (key=="intdraw"){
            var checked=$(id+" .cond_integr")[0].checked;
            var arg= (checked ? "1" : "0");
            this.cond.fbIntDraw=arg;
            optionstring +="&"+key+"="+arg;
         }
         else if (key=="xmeandraw"){
            var checked=$(id+" .cond_xmean")[0].checked;
            var arg= (checked ? "1" : "0");
            this.cond.fbXMeanDraw=arg;
            optionstring +="&"+key+"="+arg;
         }
         else if (key=="xrmsdraw"){
            var checked=$(id+" .cond_xrms")[0].checked;
            var arg= (checked ? "1" : "0");
            this.cond.fbXRMSDraw=arg;
            optionstring +="&"+key+"="+arg;
         }
         else if (key=="ymeandraw"){
            var checked=$(id+" .cond_ymean")[0].checked;
            var arg= (checked ? "1" : "0");
            this.cond.fbYMeanDraw=arg;
            optionstring +="&"+key+"="+arg;
         }
         else if (key=="yrmsdraw"){
            var checked=$(id+" .cond_yrms")[0].checked;
            var arg= (checked ? "1" : "0");
            this.cond.fbYRMSDraw=arg;
            optionstring +="&"+key+"="+arg;
         }
         else if (key=="xmaxdraw"){
            var checked=$(id+" .cond_maxx")[0].checked;
            var arg= (checked ? "1" : "0");
            this.cond.fbXMaxDraw=arg;
            optionstring +="&"+key+"="+arg;
         }
         else if (key=="ymaxdraw"){
            var checked=$(id+" .cond_maxy")[0].checked;
            var arg= (checked ? "1" : "0");
            this.cond.fbYMaxDraw=arg;
            optionstring +="&"+key+"="+arg;
         }
         else if (key=="cmaxdraw"){
            var checked=$(id+" .cond_max")[0].checked;
            var arg= (checked ? "1" : "0");
            this.cond.fbCMaxDraw=arg;
            optionstring +="&"+key+"="+arg;
         }
         else{
            console.log("Warning: EvaluateChanges found unknown key:%s", key);
         }

      }// for index
      console.log("Resulting option string:%s", optionstring);
      return optionstring;
   }

   GO4.ConditionEditor.prototype.CheckResize = function() {}
   GO4.ConditionEditor.prototype.checkResize = function() {}


   GO4.ConditionEditor.prototype.ChangePolygonDimension = function() {
      // this only changes display of condition, not condition itself!
      // note that condition is still changed in analysis only by EvaluateChanges
      // local condition copy is unchanged until we can display it somewhere.

      if(!this.isPolyCond()) return;
      var id = "#" + this.get_main_id();
      var oldpoints=this.cond.fxCut.fNpoints;
      var npoints=$(id+" .cut_points")[0].value;
      console.log("ChangePolygonDimension with numpoints="+npoints+", oldpoints="+oldpoints);
      //if(npoints==oldpoints) return; // no dimension change, do nothing - disabled, error if we again go back to original condition dimension
      if (this.cond.fxCut != null) {

         $(id + " .cut_values tbody").html(""); // clear old contents
         if (npoints > oldpoints) {
            // insert last but one point into table:
            // first points are unchanged:
            for (i = 0; i < oldpoints - 1; i++) {
               var x = this.cond.fxCut.fX[i];
               var y = this.cond.fxCut.fY[i];
               $(id + " .cut_values tbody")
               .append(
                     "<tr><td><input type='text' value='"
                     + x
                     + "'/></td>  <td> <input type='text' value='"
                     + y + "'/>  </td></tr>");
               console.log("i:" + i + ", X=" + x + " Y=" + y);
            }
            // inserted points will reproduce values of last but one point:
            var insx = this.cond.fxCut.fX[oldpoints - 2];
            var insy = this.cond.fxCut.fY[oldpoints - 2];
            for (i = oldpoints - 1; i < npoints - 1; i++) {
               $(id + " .cut_values tbody")
               .append(
                     "<tr><td><input type='text' value='"
                     + insx
                     + "'/></td>  <td> <input type='text' value='"
                     + insy + "'/>  </td></tr>");
               console.log("i:" + i + ", X=" + insx + " Y=" + insy);
            }
            // final point is kept as last point of old polygon, should
            // match first point for closed tcutg:
            var lastx = this.cond.fxCut.fX[oldpoints - 1];
            var lasty = this.cond.fxCut.fY[oldpoints - 1];
            $(id + " .cut_values tbody").append(
                  "<tr><td><input type='text' value='" + lastx
                  + "'/></td>  <td> <input type='text' value='"
                  + lasty + "'/>  </td></tr>");
            console.log("i:" + (npoints - 1) + ", X=" + lastx + " Y=" + lasty);

         }
         else
         {
            // remove last but one point from table:
            for (i = 0; i < npoints - 1; i++) {
               var x = this.cond.fxCut.fX[i];
               var y = this.cond.fxCut.fY[i];
               $(id + " .cut_values tbody")
               .append(
                     "<tr><td><input type='text' value='"
                     + x
                     + "'/></td>  <td> <input type='text' value='"
                     + y + "'/>  </td></tr>");
               console.log("i:" + i + ", X=" + x + " Y=" + y);
            }
            // final point is kept as last point of old polygon, should
            // match first point for closed tcutg:
            var lastx = this.cond.fxCut.fX[oldpoints - 1];
            var lasty = this.cond.fxCut.fY[oldpoints - 1];
            $(id + " .cut_values tbody").append(
                  "<tr><td><input type='text' value='" + lastx
                  + "'/></td>  <td> <input type='text' value='"
                  + lasty + "'/>  </td></tr>");
            console.log("i:" + (npoints - 1) + ", X=" + lastx + " Y=" + lasty);

         }
      }
      this.MarkChanged("polygon");
   }

   GO4.ConditionEditor.prototype.refreshEditor = function() {
      var editor = this;
      var id = "#" + this.get_main_id();
      var cond = this.cond;

      $(id+" .cond_name").text(cond.fName);
      $(id+" .cond_type").text(cond._typename);


      if(cond.fbEnabled) {
         $(id+" .cond_execmode").val(0);
      }
      else{
         if(cond.fbResult)
            $(id+" .cond_execmode").val(1);
         else
            $(id+" .cond_execmode").val(2);
      }
      $(id+" .cond_execmode").selectmenu("refresh");
      $(id+" .cond_execmode").selectmenu("option", "width", "100%"); // workaround for selecmenu refresh problem (increases width each time!)
      if(cond.fbTrue)
         $(id+" .cond_invertmode").val(0);
      else
         $(id+" .cond_invertmode").val(1);

      $(id+" .cond_invertmode").selectmenu("refresh");
      $(id+" .cond_invertmode").selectmenu("option", "width", "100%"); // workaround for selecmenu refresh problem (increases width each time!)


      $(id+" .cond_xmin").val(cond.fLow1).change(function(){ editor.MarkChanged("limits")});
      $(id+" .cond_xmax").val(cond.fUp1).change(function(){ editor.MarkChanged("limits")});
      if (cond.fiDim==2) {
         $(id+" .cond_ymin").val(cond.fLow2).change(function(){editor.MarkChanged("limits")});
         $(id+" .cond_ymax").val(cond.fUp2).change(function(){ editor.MarkChanged("limits")});
      } else {
         $(id+" .cond_ymin").prop('disabled', true);
         $(id+" .cond_ymax").prop('disabled', true);
      }

      if(this.isPolyCond()) {
         $(id+" .cond_tabs").tabs( "disable", 0 ); // enable/disable by tab index
         $(id+" .cond_tabs").tabs( "enable", 1 ); // enable/disable by tab index
         if (this.cond.fxCut != null) {
            var numpoints=this.cond.fxCut.fNpoints;
            console.log("refreshEditor: npoints="+numpoints);
            $(id+" .cut_points").val(numpoints); //.change(function(){ editor.MarkChanged("polygon")});
            $(id + " .cut_values tbody").html("");


            for(i = 0; i < numpoints; i++) {
               var x= this.cond.fxCut.fX[i];
               var y= this.cond.fxCut.fY[i];
               $(id + " .cut_values tbody").append("<tr><td><input type='text' value='" + x + "'/></td>  <td> <input type='text' value='" + y + "'/>  </td></tr>");
               console.log("i:"+i+", X="+x+" Y="+y);
            }
            $(id + " .cut_values tbody").change(function(){ editor.MarkChanged("polygon")});

         }
         if(this.isEllipseCond()) {
            $(id+" .cond_tabs").tabs( "enable", 2 ); // enable/disable by tab index
            var numpoints=this.cond.fiResolution;
            $(id+" .cond_ellipse_points").val(numpoints);
            $(id+" .cond_ellipse_cx").val(cond.fdCenterX).change(function(){ editor.MarkChanged("ellicx")});
            $(id+" .cond_ellipse_cy").val(cond.fdCenterY).change(function(){ editor.MarkChanged("ellicy")});
            $(id+" .cond_ellipse_a1").val(cond.fdRadius1).change(function(){ editor.MarkChanged("ellia1")});
            $(id+" .cond_ellipse_a2").val(cond.fdRadius2).prop('disabled', cond.fbIsCircle).change(function(){ editor.MarkChanged("ellia2")});
            $(id+" .cond_ellipse_theta").val(cond.fdTheta).prop('disabled', cond.fbIsCircle).change(function(){
               editor.MarkChanged("ellith");
               $(id+" .cond_ellipse_theta_slider").slider( "option", "value", $(this)[0].value % 360);
               console.log("ellipse theta value="+$(this)[0].value);
            });


            var options = $(id+" .cond_ellipse_iscircle")[0].options;
            for ( var i = 0; i < options.length; i++){
               options[i].selected = (options[i].value == cond.fiShapeType);
            }
            $(id+" .cond_ellipse_iscircle").selectmenu('refresh', true).selectmenu("option", "width", "80%");;


            $(id + " .cond_ellipse_theta_slider")
            .slider({
               min : 0,
               max : 360,
               step : 1,
               value: cond.fdTheta,
               disabled: cond.fbIsCircle,
               change : function(event, ui) {
                  editor.MarkChanged("ellith");
                  $(id + " .cond_ellipse_theta").val(ui.value);
                  console.log("slider changed to" + ui.value);
               },
               stop : function(event, ui) {
                  editor.MarkChanged("ellith");
                  console.log("sliderstopped.");
               }
            })

            ;

         }

      }
      else
      {
         console.log("refreshEditor: - NO POLYGON CUT");
         $(id+" .cond_tabs").tabs( "enable", 0 );
         $(id+" .cond_tabs").tabs( "disable", 1 ); // enable/disable by tab index
         $(id+" .cond_tabs").tabs( "disable", 2 ); // enable/disable by tab index
      }


      $(id+" .cond_counts").text(cond.fiCounts);
      $(id+" .cond_true").text(cond.fiTrueCounts);
      $(id+" .cond_percent").text((cond.fiCounts > 0 ? 100. * cond.fiTrueCounts / cond.fiCounts : 0.).toFixed(2) + "%");


      // todo: get keywords from condition class definition
      // problem: static variables are not streamed by default

      $(id+" .cond_visible")
      .prop('checked', cond.fbVisible)
      .click(function() { cond.fbVisible = this.checked; editor.MarkChanged("visible")});
      $(id+" .cond_limits")
      .prop('checked', cond.fbLimitsDraw)
      .click(function() { cond.fbLimitsDraw = this.checked; editor.MarkChanged("limitsdraw")});

      $(id+" .cond_label")
      .prop('checked', cond.fbLabelDraw)
      .click(function() { cond.fbLabelDraw = this.checked; editor.MarkChanged("labeldraw")});


      $(id+" .cond_integr")
      .prop('checked', cond.fbIntDraw)
      .click(function() { cond.fbIntDraw = this.checked; editor.MarkChanged("intdraw")});

      $(id+" .cond_maxx")
      .prop('checked', cond.fbXMaxDraw)
      .click(function() { cond.fbXMaxDraw = this.checked; editor.MarkChanged("xmaxdraw")});

      $(id+" .cond_max")
      .prop('checked', cond.fbCMaxDraw)
      .click(function() { cond.fbCMaxDraw = this.checked; editor.MarkChanged("cmaxdraw")});

      $(id+" .cond_maxy")
      .prop('checked', cond.fbYMaxDraw)
      .click(function() { cond.fbYMaxDraw = this.checked; editor.MarkChanged("ymaxdraw")});

      $(id+" .cond_xmean")
      .prop('checked', cond.fbXMeanDraw)
      .click(function() { cond.fbXMeanDraw = this.checked; editor.MarkChanged("xmeandraw")});

      $(id+" .cond_xrms")
      .prop('checked', cond.fbXRMSDraw)
      .click(function() { cond.fbXRMSDraw = this.checked; editor.MarkChanged("xrmsdraw")});

      $(id+" .cond_ymean")
      .prop('checked', cond.fbYMeanDraw)
      .click(function() { cond.fbYMeanDraw = this.checked; editor.MarkChanged("ymeandraw")});

      $(id+" .cond_yrms")
      .prop('checked', cond.fbYRMSDraw)
      .click(function() { cond.fbYRMSDraw = this.checked; editor.MarkChanged("yrmsdraw")});


      editor.ClearChanges();
   }
  //--------- end refreshEditor


  //////////////////////////////////////////////////////////
   GO4.ConditionEditor.prototype.fillEditor = function(divid, resolve) {
      if (resolve) {
         this.setTopPainter();
      } else {
         this.SetDivId(divid); // old
      }
      var id = "#" + divid;
      var editor = this;
      var cond = this.cond;
      console.log("GO4.ConditionEditor.prototype.fillEditor " + this.cond.fName);
      // $(id).css("display","table");

      $(id+" .cond_tabs").tabs();

      $(id + " .cond_execmode").selectmenu({
         change : function(event, ui) {
            editor.MarkChanged("resultmode");
         }
      });
      $(id + " .cond_invertmode").selectmenu({
         change : function(event, ui) {
            editor.MarkChanged("invertmode");
         }
      });

      if(this.isEllipseCond()) {
         $(id+" .cond_ellipse_iscircle").selectmenu({
            change : function(event, ui) {
               cond.fiShapeType = ui.item.value;
               if(cond.fiShapeType == 2)
               {
                  // circle
                  $(id+" .cond_ellipse_a2").prop('disabled', true);
                  $(id+" .cond_ellipse_theta").prop('disabled', true);
                  $(id + " .cond_ellipse_theta_slider").slider("disable");
                  $(id + "cond_ellipse_points").prop('disabled', false);
               }
               else if(cond.fiShapeType == 3)
               {
                  // ellipse
                  $(id+" .cond_ellipse_a2").prop('disabled', false);
                  $(id+" .cond_ellipse_theta").prop('disabled', false);
                  $(id + " .cond_ellipse_theta_slider").slider("enable");
                  $(id + "cond_ellipse_points").prop('disabled', false);


               }
               else if(cond.fiShapeType == 4)
               {
                  // box
                  $(id+" .cond_ellipse_a2").prop('disabled', false);
                  $(id+" .cond_ellipse_theta").prop('disabled', false);
                  $(id + " .cond_ellipse_theta_slider").slider("enable");
                  $(id + "cond_ellipse_points").prop('disabled', true);
               }
               else
               {
                  // free style
                  $(id+" .cond_ellipse_a2").prop('disabled', true);
                  $(id+" .cond_ellipse_theta").prop('disabled', true);
                  $(id + " .cond_ellipse_theta_slider").slider("disable");
                  $(id + "cond_ellipse_points").prop('disabled', false);
               }


               editor.MarkChanged("ellishape");
            }
         })

      }

      $(id+" .buttonGetCondition")
      .button({text: false, icons: { primary: "ui-icon-blank MyButtonStyle"}})
      .click(function() {
         console.log("update item = " + editor.getItemName());
         if (JSROOT.hpainter) JSROOT.hpainter.display(editor.getItemName());
         else  console.log("hierarhy painter object not found!");
      })
      .children(":first") // select first button element, used for images
      .css('background-image', "url(" + GO4.source_dir + "icons/right.png)");



      $(id+" .buttonSetCondition")
      .button({text: false, icons: { primary: "ui-icon-blank MyButtonStyle"}})
      .click(function() {
         var options=""; // do not need to use name here
         options=editor.EvaluateChanges(options); // complete option string from all changed elements
         console.log("set - condition "+ editor.getItemName()+ ", options="+options);
         GO4.ExecuteMethod(editor,"UpdateFromUrl",options,function(result) {
            console.log(result ? "set condition done. " : "set condition FAILED.");
            if(result) editor.ClearChanges();
         });
      })
      .children(":first") // select first button element, used for images
      .css('background-image', "url(" + GO4.source_dir + "icons/left.png)");


      $(id+" .buttonChangeLabel")
         .button({text: false, icons: { primary: "ui-icon-blank MyButtonStyle"}})
         .click()
         .children(":first") // select first button element, used for images
         .css('background-image', "url(" + GO4.source_dir + "icons/info1.png)");

      $(id+" .buttonDrawCondition")
         .button({text: false, icons: { primary: "ui-icon-blank MyButtonStyle"}})
         .click(function() {
            // TODO: implement correctly after MDI is improved, need to find out active frame and location of bound histogram

            if (JSROOT.hpainter) {
               editor.EvaluateChanges("");

               if (JSROOT.hpainter.updateOnOtherFrames(editor, editor.cond)) return;

               JSROOT.hpainter.drawOnSuitableHistogram(editor, editor.cond, editor.cond.fiDim==2);

               return;
            }

            //if (JSROOT.hpainter){
            //var onlineprop = JSROOT.hpainter.getOnlineProp(editor.getItemName());
            //var baseurl = onlineprop.server + onlineprop.itemname + "/";
            var baseurl = editor.getItemName() + "/";
            var drawurl = baseurl + "draw.htm", editorurl = baseurl + "draw.htm?opt=editor";
            console.log("draw condition to next window with url="+drawurl);
            //window.open(drawurl);
            window.open(drawurl,'_blank');
   //       }
   //       else
   //       {
   //       console.log("hpainter object not found!");
   //       }
   //       problem: we do not have method to get currently selected pad...
   //       var nextid="#"+(editor.divid + 1); // does not work, id is string and not number here
   //       console.log("draw condition to id="+nextid);
   //       GO4.drawGo4Cond(nextid, editor.cond, "");


         })
      .children(":first") // select first button element, used for images
      .css('background-image', "url(" + GO4.source_dir + "icons/chart.png)");


      $(id+" .buttonClearCondition")
        .button({text: false, icons: { primary: "ui-icon-blank MyButtonStyle"}})
        .click(function() {
         console.log("clearing counters...");
         var options="&resetcounters=1";
         GO4.ExecuteMethod(editor, "UpdateFromUrl",options,function(result) {
            console.log(result ? "reset condition counters done. " : "reset condition counters FAILED.");
            if (result) {
               if(JSROOT.hpainter) JSROOT.hpainter.display(editor.getItemName());
               else console.log("hpainter object not found!");
            }
         });
        })
        .children(":first") // select first button element, used for images
        .css('background-image', "url(" + GO4.source_dir + "icons/clear.png)");

      $(id+" .cut_points").spinner({
         min: 0,
         max: 1000,
         step: 1,
         //spin: function( event, ui ) {console.log("cut spin has value:"+ui.value);},
         change: function( event, ui ) {editor.ChangePolygonDimension();//console.log("cut changed.");
         },
         stop: function( event, ui ) {editor.ChangePolygonDimension();//console.log("cut spin stopped.");
         }
      });

      $(id+" .cond_ellipse_points").spinner({
         min: 0,
         max: 1000,
         step: 1,
         //spin: function( event, ui ) {console.log("cut spin has value:"+ui.value);},
         change: function( event, ui ) {editor.MarkChanged("ellinpts");console.log("ellipse points changed.");
         },
         stop: function( event, ui ) {editor.MarkChanged("ellinpts");console.log("ellipse points stopped.");
         }
      });

      this.refreshEditor();

      if (resolve) resolve(this);
              else this.DrawingReady();

      //$(document).tooltip();
      //$(id).tooltip(); // NOTE: jquery ui tooltips will change title information, currently conflict with jsroot!
   }

   GO4.ConditionEditor.prototype.drawEditor = function(divid, resolve) {
      $("#"+divid)
         .empty()
         .load(GO4.source_dir + "html/condeditor.htm", "", () => this.fillEditor(divid, resolve));
      return this;
   }

   if (JSROOT._) {
      GO4.ConditionEditor.prototype.redrawObject = function(obj/*, opt */) {
         if (obj._typename != this.cond._typename) return false;
         this.cond = JSROOT.clone(obj); // does this also work with polygon condition?
         this.refreshEditor();
         return true;
      }
   } else {
      // old style, new jsroot does not have RedrawPad for BasePainter
      GO4.ConditionEditor.prototype.RedrawPad = function(resize) {
         this.refreshEditor();
      }
      // makes sense only in jsroot v5, in v6 should be defined redrawObject
      GO4.ConditionEditor.prototype.UpdateObject = function(obj) {
         if (obj._typename != this.cond._typename) return false;
         this.cond = JSROOT.clone(obj); // does this also work with polygon condition?
         return true;
      }
   }

   // done from central go4.js or go4canvas.js scripts
   // jsrp.addDrawFunc({ name: "TGo4WinCond", func: GO4.drawGo4Cond, opt: ";editor" });
   // jsrp.addDrawFunc({ name: "TGo4PolyCond", func: GO4.drawGo4Cond, opt: ";editor" });
   // jsrp.addDrawFunc({ name: "TGo4ShapedCond", func: GO4.drawGo4Cond, opt: ";editor" });

})(); // function
