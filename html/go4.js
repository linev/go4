(function(){

   if (typeof JSROOT != "object") {
      var e1 = new Error("go4.js requires JSROOT to be already loaded");
      e1.source = "go4.js";
      throw e1;
   }

  if (typeof GO4 == "object") {
      var e1 = new Error("GO4 already defined when loading go4.js");
      e1.source = "go4.js";
      throw e1;
   }

   GO4 = {};

   GO4.version = "6.1.x";

   // use location to load all other scripts when required
   GO4.source_dir = function() {
      var scripts = document.getElementsByTagName('script');

      for (var n in scripts) {
         if (scripts[n]['type'] != 'text/javascript') continue;

         var src = scripts[n]['src'];
         if ((src == null) || (src.length == 0)) continue;

         var pos = src.indexOf("html/go4.js");
         if (pos<0) continue;
         if (src.indexOf("JSRootCore")>=0) continue;
         console.log('Set GO4.source_dir to ' + src.substr(0, pos));
         return src.substr(0, pos);
      }
      return "";
   }();

   let BasePainter = JSROOT.BasePainter || JSROOT.TBasePainter;

   if (!BasePainter.prototype.get_main_id) {
      GO4.id_counter = 1;
      // method removed from JSROOT v6, is not required there, therefore reintroduce it here
      BasePainter.prototype.get_main_id = function() {
         var elem = this.select_main();
         if (elem.empty()) return "";
         var id = elem.attr("id");
         if (!id) {
            id = "go4_element_" + GO4.id_counter++;
            elem.attr("id", id);
         }
         return id;
      }
   }

   if (typeof JSROOT.httpRequest == 'function')
      GO4.httpRequest = JSROOT.httpRequest;
   else
      GO4.httpRequest = function(url, kind, post_data) {
         return new Promise((resolveFunc,rejectFunc) => {
            let req = JSROOT.NewHttpRequest(url,kind, (res) => {
               if (res === null)
                  rejectFunc(Error(`Fail to request ${url}`));
               else
                  resolveFunc(res);
            });

            req.send(post_data || null);
         });
      }

   GO4.ExecuteMethod = function(item, method, options, callback) {
      var prefix = "";
      if (item.GetItemName())
         prefix = item.GetItemName() + "/"; // suppress / if item name is empty
      prefix += "exe.json\?method=";

      var fullcom = prefix + method + (options || "&"); // send any arguments otherwise ROOT refuse to process it

      GO4.httpRequest(fullcom, 'text')
         .then(() => callback(true))
         .catch(() => callback(false))
         .finally(() => console.log('Command is completed ' + prefix + method));
   }

   // ==================================================================================

   GO4.DrawAnalysisRatemeter = function(divid, itemname) {

      var html = "<div style='padding-top:2px'>";
      html += "<img class='go4_logo' style='vertical-align:middle;margin-left:5px;margin-right:5px;' src='go4sys/icons/go4logorun4.gif' alt='logo'></img>";
      html += "<label class='event_source' style='border: 1px solid gray; font-size:large; vertical-align:middle; padding-left:3px; padding-right:3px;'>file.lmd</label> ";
      html += "<label class='event_rate' style='border: 1px solid gray; font-size:large; vertical-align:middle; background-color: grey'; padding-left:3px; padding-right:3px;>---</label> Ev/s ";
      html += "<label class='aver_rate' style='border: 1px solid gray; font-size:large; vertical-align:middle; padding-left:3px; padding-right:3px;'>---</label> Ev/s ";
      html += "<label class='run_time' style='border: 1px solid gray; font-size:large; vertical-align:middle; padding-left:3px; padding-right:3px;'>---</label> s ";
      html += "<label class='total_events' style='border: 1px solid gray; font-size:large; vertical-align:middle; padding-left:3px; padding-right:3px;'>---</label> Events ";
      html += "<label class='analysis_time' style='border: 1px solid gray; font-size:large; vertical-align:middle; padding-left:3px; padding-right:3px;'>time</label>";
      html += "</div>";

      $('#'+divid).css('overflow','hidden')
                  .css('padding-left','5px')
                  .css('display', 'inline-block')
                  .css('white-space', 'nowrap')
                  .html(html);

      // use height of child element
      var brlayout = JSROOT.hpainter ? JSROOT.hpainter.brlayout : null;
      if (brlayout) brlayout.AdjustSeparator(null, $('#'+divid+' div').height()+4, true);

      var xreq = false, was_running = null;

      function UpdateRatemeter() {
         if (xreq) return;
         xreq = true;
         GO4.httpRequest(itemname+"/root.json.gz", 'object').then(res => {
            $('#'+divid + " .event_rate").css('background-color', res.fbRunning ? 'lightgreen' : 'red');
            if (was_running != res.fbRunning)
               $('#'+divid + " .go4_logo").attr("src", res.fbRunning ? 'go4sys/icons/go4logorun4.gif' : 'go4sys/icons/go4logo_t.png');

            was_running = res.fbRunning;

            $('#'+divid + " .event_source").text(res.fxEventSource == "" ? "<source>" : res.fxEventSource);
            $('#'+divid + " .event_rate").text(res.fdRate.toFixed(1));
            $('#'+divid + " .aver_rate").text((res.fdTime > 0 ? res.fuCurrentCount / res.fdTime : 0).toFixed(1));
            $('#'+divid + " .run_time").text(res.fdTime.toFixed(1));
            $('#'+divid + " .total_events").text(res.fuCurrentCount);
            $('#'+divid + " .analysis_time").text(res.fxDateString == "" ? "<datime>" : res.fxDateString);
         }).catch(() => {
            $('#'+divid + " .event_rate").css('background-color','grey');
         }).finally(() => {
            xreq = false;
         });
      }

      setInterval(UpdateRatemeter, 2000);
   }


   GO4.MakeMsgListRequest = function(hitem, item) {
      var arg = "&max=2000";
      if ('last-id' in item) arg+= "&id="+item['last-id'];
      return 'exe.json.gz?method=Select' + arg;
   }

   GO4.AfterMsgListRequest = function(hitem, item, obj) {
      if (!item) return;

      if (!obj) {
         delete item['last-id'];
         return;
      }
      // ignore all other classes
      if (obj._typename != 'TList') return;

      obj._typename = "TGo4MsgList";

      if (obj.arr.length>0) {
         var duplicate = (('last-id' in item) && (item['last-id'] == obj.arr[0].fString));

         item['last-id'] = obj.arr[0].fString;

         // workaround for snapshot, it sends always same messages many times
         if (duplicate) obj.arr.length = 1;

         // add clear function for item
         if (!('clear' in item))
            item['clear'] = function() { delete this['last-id']; }
      }
   }


   GO4.MsgListPainter = function(lst) {
      BasePainter.call(this);

      this.lst = lst;

      return this;
   }

   GO4.MsgListPainter.prototype = Object.create( BasePainter.prototype );

   GO4.MsgListPainter.prototype.RedrawObject = function(obj) {
      this.lst = obj;
      this.Draw();
      return true;
   }

   GO4.MsgListPainter.prototype.Draw = function() {
      if (!this.lst) return;

      var frame = this.select_main();

      var main = frame.select("div");
      if (main.empty())
         main = frame.append("div")
                     .style('max-width','100%')
                     .style('max-height','100%')
                     .style('overflow','auto');

      var old = main.selectAll("pre");
      var newsize = old.size() + this.lst.arr.length - 1;

      // in the browser keep maximum 2000 entries
      if (newsize > 2000)
         old.select(function(d,i) { return i < newsize - 2000 ? this : null; }).remove();

      for (var i=this.lst.arr.length-1;i>0;i--)
         main.append("pre").style('margin','3px').html(this.lst.arr[i].fString);

      // (re) set painter to first child element
      this.SetDivId(this.divid);

   }

   GO4.DrawMsgList = function(divid, lst, opt) {
      var painter = new GO4.MsgListPainter(lst);
      painter.SetDivId(divid);
      painter.Draw();
      return painter.DrawingReady();
   }

   GO4.drawAnalysisTerminal = function(hpainter, itemname) {
      var url = "", mdi, frame;
      // FIXME: only for short backward compatibility with jsroot5
      if (JSROOT._) {
         url = hpainter.getOnlineItemUrl(itemname);
         mdi = hpainter.getDisplay();
         if (mdi) frame = mdi.findFrame(itemname, true);
      } else {
         url = hpainter.GetOnlineItemUrl(itemname);
         mdi = hpainter.GetDisplay();
         if (mdi) frame = mdi.FindFrame(itemname, true);
      }

      if (!url || !frame) return null;

      var divid = d3.select(frame).attr('id');

      var h = $("#"+divid).height(), w = $("#"+divid).width();
      if ((h<10) && (w>10)) $("#"+divid).height(w*0.7);

      var player = new BasePainter();
      player.url = url;
      player.hpainter = hpainter;
      player.itemname = itemname;
      player.draw_ready = true;
      player.needscroll = false;

      player.DrawReady = function() {
         if(this.needscroll) {
            this.ClickScroll();
            this.needscroll = false;
         }
         this.draw_ready = true;
      }

      player.ProcessTimer = function() {
         var subid = "anaterm_output_container";
         if ($("#" + subid).length == 0) {
            // detect if drawing disappear
            clearInterval(this.interval);
            this.interval = null;
            return;
         }
         if (!this.draw_ready)
            return;

         var msgitem = this.itemname.replace("Control/Terminal", "Status/Log");

         this.draw_ready = false;

         if (JSROOT._)
            this.hpainter.display(msgitem, "divid:" + subid).then(() => this.DrawReady());
         else
            this.hpainter.display(msgitem, "divid:" + subid, this.DrawReady.bind(this));
      }

      player.ClickCommand = function(kind) {
         var pthis = this;
         this.hpainter.ExecuteCommand(this.itemname.replace("Terminal", "CmdExecute"), function() {
            pthis.needscroll = true;
         }, kind);
      }

      player.ClickClear = function() {
         document.getElementById("anaterm_output_container").firstChild.innerHTML = "";
      }

      player.ClickScroll = function() {
         //  inner frame created by hpainter has the scrollbars, i.e. first child
         var disp = $("#anaterm_output_container").children(":first");
         disp.scrollTop(disp[0].scrollHeight - disp.height());
      }


      player.fillDisplay = function(id) {

          this.SetDivId(id);
          this.interval = setInterval(this.ProcessTimer.bind(this), 2000);

          id = "#" + id; // to use in jQuery

          $(id + " .go4_clearterm")
              .button({text: false, icons: { primary: "ui-icon-blank MyTermButtonStyle"}})
              .click(this.ClickClear.bind(this))
              .children(":first") // select first button element, used for images
              .css('background-image', "url(" + GO4.source_dir + "icons/clear.png)");

          $(id + " .go4_endterm")
            .button({text: false, icons: { primary: "ui-icon-blank MyTermButtonStyle"}})
            .click(this.ClickScroll.bind(this))
            .children(":first") // select first button element, used for images
            .css('background-image', "url(" + GO4.source_dir + "icons/shiftdown.png)");

          $(id + " .go4_printhistos")
             .button({text: false, icons: { primary: "ui-icon-blank MyTermButtonStyle"}})
             .click(this.ClickCommand.bind(this,"@PrintHistograms()"))
             .children(":first") // select first button element, used for images
             .css('background-image', "url(" + GO4.source_dir + "icons/hislist.png)");

          $(id + " .go4_printcond")
             .button({text: false, icons: { primary: "ui-icon-blank MyTermButtonStyle"}})
             .click(this.ClickCommand.bind(this,"@PrintConditions()"))
             .children(":first") // select first button element, used for images
             .css('background-image', "url(" + GO4.source_dir + "icons/condlist.png)");

          var pthis = this;

          $("#go4_anaterm_cmd_form").submit(
              function(event) {
                 var command = pthis.itemname.replace("Terminal", "CmdExecute");
                 var cmdpar=document.getElementById("go4_anaterm_command").value;
                 console.log("submit command - " + cmdpar);
                 pthis.hpainter.ExecuteCommand(command,  function() { pthis.needscroll=true; }, cmdpar);
                 event.preventDefault();
              });

         $(id + " .go4_executescript")
            .button({text: false, icons: { primary: "ui-icon-blank MyTermButtonStyle"}})
            .children(":first") // select first button element, used for images
            .css('background-image', "url(" + GO4.source_dir + "icons/macro_t.png)");
      }

      player.CheckResize = player.checkResize = function() {}

      $("#"+divid).load(GO4.source_dir + "html/terminal.htm", "", player.fillDisplay.bind(player, divid));

      return player;
   }


   // ==============================================================================

   var canvsrc = GO4.source_dir + 'html/go4canvas.js;';

   JSROOT.addDrawFunc({ name: "TGo4WinCond",  script: canvsrc + GO4.source_dir + 'html/condition.js', func: 'GO4.drawGo4Cond', opt: ";editor" });
   JSROOT.addDrawFunc({ name: "TGo4PolyCond", script: canvsrc + GO4.source_dir + 'html/condition.js', func: 'GO4.drawGo4Cond', opt: ";editor" });
   JSROOT.addDrawFunc({ name: "TGo4ShapedCond", script: canvsrc + GO4.source_dir + 'html/condition.js', func: 'GO4.drawGo4Cond', opt: ";editor" });
   JSROOT.addDrawFunc({ name: "TGo4Marker", script: canvsrc, func: 'GO4.drawGo4Marker' });

   JSROOT.addDrawFunc({ name: "TGo4AnalysisWebStatus", script: GO4.source_dir + 'html/analysiseditor.js', func: 'GO4.drawGo4AnalysisStatus', opt: "editor" });

   JSROOT.addDrawFunc({ name: "TGo4MsgList", func: GO4.DrawMsgList });

   JSROOT.addDrawFunc({ name: "TGo4MbsEvent", noinspect: true });
   JSROOT.addDrawFunc({ name: "TGo4EventElement", noinspect: true });

})(); // factory
