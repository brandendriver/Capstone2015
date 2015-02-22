<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="7.1.0">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="10" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="26" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="con-molex">
<description>&lt;b&gt;Molex Connectors&lt;/b&gt;&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="7395-02">
<description>&lt;b&gt;MOLEX 2.54mm KK RA CONNECTOR&lt;/b&gt;
&lt;br&gt;Fixed Orientation</description>
<wire x1="2.54" y1="-5.08" x2="2.54" y2="-1.905" width="0.254" layer="21"/>
<wire x1="2.54" y1="-1.905" x2="1.905" y2="-1.905" width="0.254" layer="21"/>
<wire x1="1.905" y1="-1.905" x2="1.905" y2="-2.54" width="0.254" layer="21"/>
<wire x1="1.905" y1="-2.54" x2="0.635" y2="-2.54" width="0.254" layer="21"/>
<wire x1="0.635" y1="-2.54" x2="0.635" y2="-1.905" width="0.254" layer="21"/>
<wire x1="0.635" y1="-1.905" x2="-0.635" y2="-1.905" width="0.254" layer="21"/>
<wire x1="-0.635" y1="-1.905" x2="-0.635" y2="-2.54" width="0.254" layer="21"/>
<wire x1="-0.635" y1="-2.54" x2="-1.905" y2="-2.54" width="0.254" layer="21"/>
<wire x1="-1.905" y1="-2.54" x2="-1.905" y2="-1.905" width="0.254" layer="21"/>
<wire x1="-1.905" y1="-1.905" x2="-2.54" y2="-1.905" width="0.254" layer="21"/>
<wire x1="-2.54" y1="-1.905" x2="-2.54" y2="-5.08" width="0.254" layer="21"/>
<wire x1="-2.54" y1="-5.08" x2="-1.651" y2="-5.08" width="0.254" layer="21"/>
<wire x1="-1.651" y1="-5.08" x2="1.27" y2="-5.08" width="0.254" layer="21"/>
<wire x1="1.27" y1="-5.08" x2="2.54" y2="-5.08" width="0.254" layer="21"/>
<wire x1="1.27" y1="-5.08" x2="1.27" y2="-13.081" width="0.254" layer="21"/>
<wire x1="1.27" y1="-13.081" x2="1.016" y2="-13.589" width="0.254" layer="21"/>
<wire x1="1.016" y1="-13.589" x2="-0.0508" y2="-13.589" width="0.254" layer="21"/>
<wire x1="-1.27" y1="-5.08" x2="-1.27" y2="-13.081" width="0.254" layer="21"/>
<wire x1="-1.27" y1="-13.081" x2="-1.016" y2="-13.589" width="0.254" layer="21"/>
<wire x1="-1.016" y1="-13.589" x2="-0.0508" y2="-13.589" width="0.254" layer="21"/>
<wire x1="-0.0508" y1="-13.589" x2="0.0508" y2="-13.589" width="0.254" layer="21"/>
<wire x1="1.27" y1="-5.08" x2="0.889" y2="-5.08" width="0.254" layer="51"/>
<wire x1="-1.651" y1="-5.08" x2="-1.651" y2="-11.303" width="0.254" layer="21"/>
<wire x1="-1.651" y1="-11.303" x2="-1.3208" y2="-11.6586" width="0.254" layer="21"/>
<wire x1="1.651" y1="-5.08" x2="1.651" y2="-11.303" width="0.254" layer="21"/>
<wire x1="1.651" y1="-11.303" x2="1.3208" y2="-11.6586" width="0.254" layer="21"/>
<pad name="1" x="1.27" y="0" drill="1.016" shape="long" rot="R90"/>
<pad name="2" x="-1.27" y="0" drill="1.016" shape="long" rot="R90"/>
<text x="2.6401" y="2.9479" size="1.016" layer="25" ratio="10" rot="R180">&gt;NAME</text>
<text x="0.7859" y="-3.4021" size="1.27" layer="21" ratio="14" rot="R270">1</text>
<text x="-1.7033" y="-3.2751" size="1.27" layer="21" ratio="14" rot="R270">2</text>
<text x="3.4021" y="-2.4399" size="0.8128" layer="27" ratio="10" rot="R270">&gt;VALUE</text>
<rectangle x1="1.016" y1="-0.254" x2="1.524" y2="0.254" layer="51"/>
<rectangle x1="-1.524" y1="-0.254" x2="-1.016" y2="0.254" layer="51"/>
<rectangle x1="1.016" y1="-2.5146" x2="1.524" y2="-0.2794" layer="51"/>
<rectangle x1="-1.524" y1="-2.5146" x2="-1.016" y2="-0.2794" layer="51"/>
</package>
<package name="6410-02">
<description>&lt;b&gt;MOLEX 2.54mm KK  CONNECTOR&lt;/b&gt;</description>
<wire x1="-1.27" y1="2.54" x2="-1.27" y2="2.921" width="0.127" layer="21"/>
<wire x1="-2.54" y1="0" x2="-2.54" y2="2.921" width="0.254" layer="21"/>
<wire x1="-2.54" y1="2.921" x2="-1.27" y2="2.921" width="0.254" layer="21"/>
<wire x1="-1.016" y1="2.921" x2="-1.27" y2="2.921" width="0.127" layer="21"/>
<wire x1="-1.27" y1="2.921" x2="0" y2="2.921" width="0.254" layer="21"/>
<wire x1="2.54" y1="0" x2="2.54" y2="2.921" width="0.254" layer="21"/>
<wire x1="2.54" y1="2.921" x2="0" y2="2.921" width="0.127" layer="21"/>
<wire x1="-2.54" y1="0" x2="-2.54" y2="-2.921" width="0.254" layer="21"/>
<wire x1="-2.54" y1="-2.921" x2="0" y2="-2.921" width="0.254" layer="21"/>
<wire x1="2.54" y1="0" x2="2.54" y2="-2.921" width="0.254" layer="21"/>
<wire x1="2.54" y1="-2.921" x2="1.905" y2="-2.921" width="0.127" layer="21"/>
<wire x1="1.905" y1="-2.921" x2="0.635" y2="-2.921" width="0.254" layer="21"/>
<wire x1="0.635" y1="-2.921" x2="0" y2="-2.921" width="0.127" layer="21"/>
<wire x1="-2.286" y1="-2.921" x2="-1.905" y2="-2.921" width="0.127" layer="21"/>
<wire x1="-2.54" y1="-2.921" x2="-1.905" y2="-2.921" width="0.254" layer="21"/>
<wire x1="-1.905" y1="-2.921" x2="-1.905" y2="-2.286" width="0.254" layer="21"/>
<wire x1="-1.905" y1="-2.286" x2="-0.635" y2="-2.286" width="0.254" layer="21"/>
<wire x1="-0.635" y1="-2.286" x2="-0.635" y2="-2.921" width="0.254" layer="21"/>
<wire x1="-0.635" y1="-2.921" x2="0.635" y2="-2.921" width="0.254" layer="21"/>
<wire x1="0.635" y1="-2.921" x2="0.635" y2="-2.286" width="0.254" layer="21"/>
<wire x1="0.635" y1="-2.286" x2="1.905" y2="-2.286" width="0.254" layer="21"/>
<wire x1="1.905" y1="-2.286" x2="1.905" y2="-2.921" width="0.254" layer="21"/>
<wire x1="1.905" y1="-2.921" x2="2.54" y2="-2.921" width="0.254" layer="21"/>
<wire x1="-1.27" y1="1.905" x2="-1.27" y2="2.921" width="0.254" layer="21"/>
<wire x1="-1.27" y1="1.905" x2="-1.016" y2="1.905" width="0.254" layer="21"/>
<wire x1="-1.016" y1="1.905" x2="1.016" y2="1.905" width="0.254" layer="21"/>
<wire x1="1.016" y1="1.905" x2="1.27" y2="1.905" width="0.254" layer="21"/>
<wire x1="1.27" y1="1.905" x2="1.27" y2="2.921" width="0.254" layer="21"/>
<wire x1="-1.27" y1="1.905" x2="-1.016" y2="1.397" width="0.254" layer="21"/>
<wire x1="-1.016" y1="2.921" x2="-1.016" y2="1.905" width="0.254" layer="21"/>
<wire x1="-1.016" y1="1.397" x2="1.016" y2="1.397" width="0.254" layer="21"/>
<wire x1="1.016" y1="1.397" x2="1.27" y2="1.905" width="0.254" layer="21"/>
<wire x1="1.016" y1="2.921" x2="1.016" y2="1.905" width="0.254" layer="21"/>
<pad name="2" x="-1.27" y="0" drill="1.016" shape="long" rot="R90"/>
<pad name="1" x="1.27" y="0" drill="1.016" shape="long" rot="R90"/>
<text x="-2.5131" y="3.2751" size="1.016" layer="25" ratio="10">&gt;NAME</text>
<text x="3.2751" y="-0.6111" size="1.27" layer="21" ratio="14">1</text>
<text x="-3.7099" y="-0.6873" size="1.27" layer="21" ratio="14">2</text>
<text x="-2.5131" y="-4.5989" size="0.8128" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="-1.524" y1="-0.254" x2="-1.016" y2="0.254" layer="51"/>
<rectangle x1="1.016" y1="-0.254" x2="1.524" y2="0.254" layer="51"/>
</package>
<package name="87758-2616">
<description>&lt;b&gt;26 Pin - 2mm Dual Row Single Wafer, Vertical T/H HDR&lt;/b&gt;&lt;p&gt;
Source: http://www.molex.com/pdm_docs/sd/877580616_sd.pdf</description>
<wire x1="-12.85" y1="-1.9" x2="12.85" y2="-1.9" width="0.2032" layer="21"/>
<wire x1="12.85" y1="-1.9" x2="12.85" y2="-0.4" width="0.2032" layer="21"/>
<wire x1="12.85" y1="0.4" x2="12.85" y2="1.9" width="0.2032" layer="21"/>
<wire x1="12.85" y1="1.9" x2="-12.85" y2="1.9" width="0.2032" layer="21"/>
<wire x1="-12.85" y1="1.9" x2="-12.85" y2="0.4" width="0.2032" layer="21"/>
<wire x1="-12.85" y1="-0.4" x2="-12.85" y2="-1.9" width="0.2032" layer="21"/>
<wire x1="-12.85" y1="0.4" x2="-12.85" y2="-0.4" width="0.2032" layer="21" curve="-129.184564"/>
<wire x1="12.85" y1="-0.4" x2="12.85" y2="0.4" width="0.2032" layer="21" curve="-129.184564"/>
<pad name="1" x="-12" y="-1" drill="0.9" diameter="1.27"/>
<pad name="2" x="-12" y="1" drill="0.9" diameter="1.27"/>
<pad name="3" x="-10" y="-1" drill="0.9" diameter="1.27"/>
<pad name="4" x="-10" y="1" drill="0.9" diameter="1.27"/>
<pad name="5" x="-8" y="-1" drill="0.9" diameter="1.27"/>
<pad name="6" x="-8" y="1" drill="0.9" diameter="1.27"/>
<pad name="7" x="-6" y="-1" drill="0.9" diameter="1.27"/>
<pad name="8" x="-6" y="1" drill="0.9" diameter="1.27"/>
<pad name="9" x="-4" y="-1" drill="0.9" diameter="1.27"/>
<pad name="10" x="-4" y="1" drill="0.9" diameter="1.27"/>
<pad name="11" x="-2" y="-1" drill="0.9" diameter="1.27"/>
<pad name="12" x="-2" y="1" drill="0.9" diameter="1.27"/>
<pad name="13" x="0" y="-1" drill="0.9" diameter="1.27"/>
<pad name="14" x="0" y="1" drill="0.9" diameter="1.27"/>
<pad name="15" x="2" y="-1" drill="0.9" diameter="1.27"/>
<pad name="16" x="2" y="1" drill="0.9" diameter="1.27"/>
<pad name="17" x="4" y="-1" drill="0.9" diameter="1.27"/>
<pad name="18" x="4" y="1" drill="0.9" diameter="1.27"/>
<pad name="19" x="6" y="-1" drill="0.9" diameter="1.27"/>
<pad name="20" x="6" y="1" drill="0.9" diameter="1.27"/>
<pad name="21" x="8" y="-1" drill="0.9" diameter="1.27"/>
<pad name="22" x="8" y="1" drill="0.9" diameter="1.27"/>
<pad name="23" x="10" y="-1" drill="0.9" diameter="1.27"/>
<pad name="24" x="10" y="1" drill="0.9" diameter="1.27"/>
<pad name="25" x="12" y="-1" drill="0.9" diameter="1.27"/>
<pad name="26" x="12" y="1" drill="0.9" diameter="1.27"/>
<text x="-12.65" y="-1.75" size="0.3048" layer="21" font="vector">1</text>
<text x="-12.62" y="-3.81" size="1.27" layer="25">&gt;NAME</text>
<text x="-6.27" y="-3.81" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-12.25" y1="-1.25" x2="-11.75" y2="-0.75" layer="51"/>
<rectangle x1="-12.25" y1="0.75" x2="-11.75" y2="1.25" layer="51"/>
<rectangle x1="-10.25" y1="-1.25" x2="-9.75" y2="-0.75" layer="51"/>
<rectangle x1="-10.25" y1="0.75" x2="-9.75" y2="1.25" layer="51"/>
<rectangle x1="-8.25" y1="-1.25" x2="-7.75" y2="-0.75" layer="51"/>
<rectangle x1="-8.25" y1="0.75" x2="-7.75" y2="1.25" layer="51"/>
<rectangle x1="-6.25" y1="-1.25" x2="-5.75" y2="-0.75" layer="51"/>
<rectangle x1="-6.25" y1="0.75" x2="-5.75" y2="1.25" layer="51"/>
<rectangle x1="-4.25" y1="-1.25" x2="-3.75" y2="-0.75" layer="51"/>
<rectangle x1="-4.25" y1="0.75" x2="-3.75" y2="1.25" layer="51"/>
<rectangle x1="-2.25" y1="-1.25" x2="-1.75" y2="-0.75" layer="51"/>
<rectangle x1="-2.25" y1="0.75" x2="-1.75" y2="1.25" layer="51"/>
<rectangle x1="-0.25" y1="-1.25" x2="0.25" y2="-0.75" layer="51"/>
<rectangle x1="-0.25" y1="0.75" x2="0.25" y2="1.25" layer="51"/>
<rectangle x1="1.75" y1="-1.25" x2="2.25" y2="-0.75" layer="51"/>
<rectangle x1="1.75" y1="0.75" x2="2.25" y2="1.25" layer="51"/>
<rectangle x1="3.75" y1="-1.25" x2="4.25" y2="-0.75" layer="51"/>
<rectangle x1="3.75" y1="0.75" x2="4.25" y2="1.25" layer="51"/>
<rectangle x1="5.75" y1="-1.25" x2="6.25" y2="-0.75" layer="51"/>
<rectangle x1="5.75" y1="0.75" x2="6.25" y2="1.25" layer="51"/>
<rectangle x1="7.75" y1="-1.25" x2="8.25" y2="-0.75" layer="51"/>
<rectangle x1="7.75" y1="0.75" x2="8.25" y2="1.25" layer="51"/>
<rectangle x1="9.75" y1="-1.25" x2="10.25" y2="-0.75" layer="51"/>
<rectangle x1="9.75" y1="0.75" x2="10.25" y2="1.25" layer="51"/>
<rectangle x1="11.75" y1="-1.25" x2="12.25" y2="-0.75" layer="51"/>
<rectangle x1="11.75" y1="0.75" x2="12.25" y2="1.25" layer="51"/>
</package>
</packages>
<symbols>
<symbol name="MV">
<wire x1="1.27" y1="0" x2="0" y2="0" width="0.6096" layer="94"/>
<text x="2.54" y="-0.762" size="1.524" layer="95">&gt;NAME</text>
<text x="-0.762" y="1.397" size="1.778" layer="96">&gt;VALUE</text>
<pin name="S" x="-2.54" y="0" visible="off" length="short" direction="pas"/>
</symbol>
<symbol name="M">
<wire x1="1.27" y1="0" x2="0" y2="0" width="0.6096" layer="94"/>
<text x="2.54" y="-0.762" size="1.524" layer="95">&gt;NAME</text>
<pin name="S" x="-2.54" y="0" visible="off" length="short" direction="pas"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="22-?-02" prefix="J">
<description>&lt;b&gt;CONNECTOR&lt;/b&gt;&lt;p&gt;
wire to board 2.54 mm (.1 inch) pitch header</description>
<gates>
<gate name="-1" symbol="MV" x="0" y="0" addlevel="always" swaplevel="1"/>
<gate name="-2" symbol="M" x="0" y="-5.08" addlevel="always" swaplevel="1"/>
</gates>
<devices>
<device name="05-7028" package="7395-02">
<connects>
<connect gate="-1" pin="S" pad="1"/>
<connect gate="-2" pin="S" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OC_FARNELL" value="9731601" constant="no"/>
<attribute name="OC_NEWARK" value="unknown" constant="no"/>
</technology>
</technologies>
</device>
<device name="27-2021" package="6410-02">
<connects>
<connect gate="-1" pin="S" pad="1"/>
<connect gate="-2" pin="S" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OC_FARNELL" value="unknown" constant="no"/>
<attribute name="OC_NEWARK" value="unknown" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="87758-2616" prefix="X">
<description>&lt;b&gt;26 Pin - 2mm Dual Row Single Wafer, Vertical T/H HDR&lt;/b&gt;&lt;p&gt;
Source: http://www.molex.com/pdm_docs/sd/877580616_sd.pdf</description>
<gates>
<gate name="-1" symbol="MV" x="-10.16" y="12.7" addlevel="always"/>
<gate name="-2" symbol="MV" x="10.16" y="12.7" addlevel="always"/>
<gate name="-3" symbol="M" x="-10.16" y="10.16" addlevel="always"/>
<gate name="-4" symbol="M" x="10.16" y="10.16" addlevel="always"/>
<gate name="-5" symbol="M" x="-10.16" y="7.62" addlevel="always"/>
<gate name="-6" symbol="M" x="10.16" y="7.62" addlevel="always"/>
<gate name="-7" symbol="M" x="-10.16" y="5.08" addlevel="always"/>
<gate name="-8" symbol="M" x="10.16" y="5.08" addlevel="always"/>
<gate name="-9" symbol="M" x="-10.16" y="2.54" addlevel="always"/>
<gate name="-10" symbol="M" x="10.16" y="2.54" addlevel="always"/>
<gate name="-11" symbol="M" x="-10.16" y="0" addlevel="always"/>
<gate name="-12" symbol="M" x="10.16" y="0" addlevel="always"/>
<gate name="-13" symbol="M" x="-10.16" y="-2.54" addlevel="always"/>
<gate name="-14" symbol="M" x="10.16" y="-2.54" addlevel="always"/>
<gate name="-15" symbol="M" x="-10.16" y="-5.08" addlevel="always"/>
<gate name="-16" symbol="M" x="10.16" y="-5.08" addlevel="always"/>
<gate name="-17" symbol="M" x="-10.16" y="-7.62" addlevel="always"/>
<gate name="-18" symbol="M" x="10.16" y="-7.62" addlevel="always"/>
<gate name="-19" symbol="M" x="-10.16" y="-10.16" addlevel="always"/>
<gate name="-20" symbol="M" x="10.16" y="-10.16" addlevel="always"/>
<gate name="-21" symbol="M" x="-10.16" y="-12.7" addlevel="always"/>
<gate name="-22" symbol="M" x="10.16" y="-12.7" addlevel="always"/>
<gate name="-23" symbol="M" x="-10.16" y="-15.24" addlevel="always"/>
<gate name="-24" symbol="M" x="10.16" y="-15.24" addlevel="always"/>
<gate name="-25" symbol="M" x="-10.16" y="-17.78" addlevel="always"/>
<gate name="-26" symbol="M" x="10.16" y="-17.78" addlevel="always"/>
</gates>
<devices>
<device name="" package="87758-2616">
<connects>
<connect gate="-1" pin="S" pad="1"/>
<connect gate="-10" pin="S" pad="10"/>
<connect gate="-11" pin="S" pad="11"/>
<connect gate="-12" pin="S" pad="12"/>
<connect gate="-13" pin="S" pad="13"/>
<connect gate="-14" pin="S" pad="14"/>
<connect gate="-15" pin="S" pad="15"/>
<connect gate="-16" pin="S" pad="16"/>
<connect gate="-17" pin="S" pad="17"/>
<connect gate="-18" pin="S" pad="18"/>
<connect gate="-19" pin="S" pad="19"/>
<connect gate="-2" pin="S" pad="2"/>
<connect gate="-20" pin="S" pad="20"/>
<connect gate="-21" pin="S" pad="21"/>
<connect gate="-22" pin="S" pad="22"/>
<connect gate="-23" pin="S" pad="23"/>
<connect gate="-24" pin="S" pad="24"/>
<connect gate="-25" pin="S" pad="25"/>
<connect gate="-26" pin="S" pad="26"/>
<connect gate="-3" pin="S" pad="3"/>
<connect gate="-4" pin="S" pad="4"/>
<connect gate="-5" pin="S" pad="5"/>
<connect gate="-6" pin="S" pad="6"/>
<connect gate="-7" pin="S" pad="7"/>
<connect gate="-8" pin="S" pad="8"/>
<connect gate="-9" pin="S" pad="9"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="" constant="no"/>
<attribute name="MPN" value="87758-2616" constant="no"/>
<attribute name="OC_FARNELL" value="unknown" constant="no"/>
<attribute name="OC_NEWARK" value="94M1541" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="6" library="con-molex" deviceset="22-?-02" device="27-2021"/>
<part name="7" library="con-molex" deviceset="22-?-02" device="27-2021"/>
<part name="8" library="con-molex" deviceset="22-?-02" device="27-2021"/>
<part name="9" library="con-molex" deviceset="22-?-02" device="27-2021"/>
<part name="10" library="con-molex" deviceset="22-?-02" device="27-2021"/>
<part name="11" library="con-molex" deviceset="22-?-02" device="27-2021"/>
<part name="12" library="con-molex" deviceset="22-?-02" device="27-2021"/>
<part name="13" library="con-molex" deviceset="22-?-02" device="27-2021"/>
<part name="5" library="con-molex" deviceset="22-?-02" device="27-2021"/>
<part name="4" library="con-molex" deviceset="22-?-02" device="27-2021"/>
<part name="3" library="con-molex" deviceset="22-?-02" device="27-2021"/>
<part name="2" library="con-molex" deviceset="22-?-02" device="27-2021"/>
<part name="1" library="con-molex" deviceset="22-?-02" device="27-2021"/>
<part name="X1" library="con-molex" deviceset="87758-2616" device=""/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="6" gate="-1" x="60.96" y="86.36" rot="R180"/>
<instance part="6" gate="-2" x="60.96" y="91.44" rot="R180"/>
<instance part="7" gate="-1" x="60.96" y="76.2" rot="R180"/>
<instance part="7" gate="-2" x="60.96" y="81.28" rot="R180"/>
<instance part="8" gate="-1" x="60.96" y="66.04" rot="R180"/>
<instance part="8" gate="-2" x="60.96" y="71.12" rot="R180"/>
<instance part="9" gate="-1" x="60.96" y="55.88" rot="R180"/>
<instance part="9" gate="-2" x="60.96" y="60.96" rot="R180"/>
<instance part="10" gate="-1" x="60.96" y="45.72" rot="R180"/>
<instance part="10" gate="-2" x="60.96" y="50.8" rot="R180"/>
<instance part="11" gate="-1" x="60.96" y="35.56" rot="R180"/>
<instance part="11" gate="-2" x="60.96" y="40.64" rot="R180"/>
<instance part="12" gate="-1" x="60.96" y="25.4" rot="R180"/>
<instance part="12" gate="-2" x="60.96" y="30.48" rot="R180"/>
<instance part="13" gate="-1" x="60.96" y="15.24" rot="R180"/>
<instance part="13" gate="-2" x="60.96" y="20.32" rot="R180"/>
<instance part="5" gate="-1" x="60.96" y="96.52" rot="R180"/>
<instance part="5" gate="-2" x="60.96" y="101.6" rot="R180"/>
<instance part="4" gate="-1" x="60.96" y="106.68" rot="R180"/>
<instance part="4" gate="-2" x="60.96" y="111.76" rot="R180"/>
<instance part="3" gate="-1" x="60.96" y="116.84" rot="R180"/>
<instance part="3" gate="-2" x="60.96" y="121.92" rot="R180"/>
<instance part="2" gate="-1" x="60.96" y="127" rot="R180"/>
<instance part="2" gate="-2" x="60.96" y="132.08" rot="R180"/>
<instance part="1" gate="-1" x="60.96" y="137.16" rot="R180"/>
<instance part="1" gate="-2" x="60.96" y="142.24" rot="R180"/>
<instance part="X1" gate="-1" x="99.06" y="93.98"/>
<instance part="X1" gate="-2" x="119.38" y="93.98"/>
<instance part="X1" gate="-3" x="99.06" y="91.44"/>
<instance part="X1" gate="-4" x="119.38" y="91.44"/>
<instance part="X1" gate="-5" x="99.06" y="88.9"/>
<instance part="X1" gate="-6" x="119.38" y="88.9"/>
<instance part="X1" gate="-7" x="99.06" y="86.36"/>
<instance part="X1" gate="-8" x="119.38" y="86.36"/>
<instance part="X1" gate="-9" x="99.06" y="83.82"/>
<instance part="X1" gate="-10" x="119.38" y="83.82"/>
<instance part="X1" gate="-11" x="99.06" y="81.28"/>
<instance part="X1" gate="-12" x="119.38" y="81.28"/>
<instance part="X1" gate="-13" x="99.06" y="78.74"/>
<instance part="X1" gate="-14" x="119.38" y="78.74"/>
<instance part="X1" gate="-15" x="99.06" y="76.2"/>
<instance part="X1" gate="-16" x="119.38" y="76.2"/>
<instance part="X1" gate="-17" x="99.06" y="73.66"/>
<instance part="X1" gate="-18" x="119.38" y="73.66"/>
<instance part="X1" gate="-19" x="99.06" y="71.12"/>
<instance part="X1" gate="-20" x="119.38" y="71.12"/>
<instance part="X1" gate="-21" x="99.06" y="68.58"/>
<instance part="X1" gate="-22" x="119.38" y="68.58"/>
<instance part="X1" gate="-23" x="99.06" y="66.04"/>
<instance part="X1" gate="-24" x="119.38" y="66.04"/>
<instance part="X1" gate="-25" x="99.06" y="63.5"/>
<instance part="X1" gate="-26" x="119.38" y="63.5"/>
</instances>
<busses>
</busses>
<nets>
<net name="N$1" class="0">
<segment>
<pinref part="1" gate="-1" pin="S"/>
<pinref part="X1" gate="-1" pin="S"/>
<wire x1="63.5" y1="137.16" x2="96.52" y2="137.16" width="0.1524" layer="91"/>
<wire x1="96.52" y1="137.16" x2="96.52" y2="93.98" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$2" class="0">
<segment>
<pinref part="1" gate="-2" pin="S"/>
<pinref part="X1" gate="-2" pin="S"/>
<wire x1="63.5" y1="142.24" x2="116.84" y2="142.24" width="0.1524" layer="91"/>
<wire x1="116.84" y1="142.24" x2="116.84" y2="93.98" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$3" class="0">
<segment>
<pinref part="2" gate="-1" pin="S"/>
<wire x1="63.5" y1="127" x2="93.98" y2="127" width="0.1524" layer="91"/>
<wire x1="93.98" y1="127" x2="93.98" y2="91.44" width="0.1524" layer="91"/>
<pinref part="X1" gate="-3" pin="S"/>
<wire x1="93.98" y1="91.44" x2="96.52" y2="91.44" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$4" class="0">
<segment>
<pinref part="3" gate="-1" pin="S"/>
<wire x1="63.5" y1="116.84" x2="91.44" y2="116.84" width="0.1524" layer="91"/>
<wire x1="91.44" y1="116.84" x2="91.44" y2="88.9" width="0.1524" layer="91"/>
<pinref part="X1" gate="-5" pin="S"/>
<wire x1="91.44" y1="88.9" x2="96.52" y2="88.9" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$5" class="0">
<segment>
<pinref part="4" gate="-1" pin="S"/>
<wire x1="63.5" y1="106.68" x2="88.9" y2="106.68" width="0.1524" layer="91"/>
<wire x1="88.9" y1="106.68" x2="88.9" y2="86.36" width="0.1524" layer="91"/>
<pinref part="X1" gate="-7" pin="S"/>
<wire x1="88.9" y1="86.36" x2="96.52" y2="86.36" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$6" class="0">
<segment>
<pinref part="5" gate="-1" pin="S"/>
<wire x1="63.5" y1="96.52" x2="86.36" y2="96.52" width="0.1524" layer="91"/>
<wire x1="86.36" y1="96.52" x2="86.36" y2="83.82" width="0.1524" layer="91"/>
<pinref part="X1" gate="-9" pin="S"/>
<wire x1="86.36" y1="83.82" x2="96.52" y2="83.82" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$7" class="0">
<segment>
<pinref part="6" gate="-1" pin="S"/>
<wire x1="63.5" y1="86.36" x2="83.82" y2="86.36" width="0.1524" layer="91"/>
<wire x1="83.82" y1="86.36" x2="83.82" y2="81.28" width="0.1524" layer="91"/>
<pinref part="X1" gate="-11" pin="S"/>
<wire x1="83.82" y1="81.28" x2="96.52" y2="81.28" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$8" class="0">
<segment>
<pinref part="7" gate="-1" pin="S"/>
<wire x1="63.5" y1="76.2" x2="81.28" y2="76.2" width="0.1524" layer="91"/>
<wire x1="81.28" y1="76.2" x2="81.28" y2="78.74" width="0.1524" layer="91"/>
<pinref part="X1" gate="-13" pin="S"/>
<wire x1="81.28" y1="78.74" x2="96.52" y2="78.74" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$9" class="0">
<segment>
<pinref part="8" gate="-1" pin="S"/>
<wire x1="63.5" y1="66.04" x2="83.82" y2="66.04" width="0.1524" layer="91"/>
<wire x1="83.82" y1="66.04" x2="83.82" y2="76.2" width="0.1524" layer="91"/>
<pinref part="X1" gate="-15" pin="S"/>
<wire x1="83.82" y1="76.2" x2="96.52" y2="76.2" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$10" class="0">
<segment>
<pinref part="9" gate="-1" pin="S"/>
<wire x1="63.5" y1="55.88" x2="86.36" y2="55.88" width="0.1524" layer="91"/>
<wire x1="86.36" y1="55.88" x2="86.36" y2="73.66" width="0.1524" layer="91"/>
<pinref part="X1" gate="-17" pin="S"/>
<wire x1="86.36" y1="73.66" x2="96.52" y2="73.66" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$11" class="0">
<segment>
<pinref part="10" gate="-1" pin="S"/>
<wire x1="63.5" y1="45.72" x2="88.9" y2="45.72" width="0.1524" layer="91"/>
<wire x1="88.9" y1="45.72" x2="88.9" y2="71.12" width="0.1524" layer="91"/>
<pinref part="X1" gate="-19" pin="S"/>
<wire x1="88.9" y1="71.12" x2="96.52" y2="71.12" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$12" class="0">
<segment>
<pinref part="11" gate="-1" pin="S"/>
<wire x1="63.5" y1="35.56" x2="91.44" y2="35.56" width="0.1524" layer="91"/>
<wire x1="91.44" y1="35.56" x2="91.44" y2="68.58" width="0.1524" layer="91"/>
<pinref part="X1" gate="-21" pin="S"/>
<wire x1="91.44" y1="68.58" x2="96.52" y2="68.58" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$13" class="0">
<segment>
<pinref part="12" gate="-1" pin="S"/>
<wire x1="63.5" y1="25.4" x2="93.98" y2="25.4" width="0.1524" layer="91"/>
<wire x1="93.98" y1="25.4" x2="93.98" y2="66.04" width="0.1524" layer="91"/>
<pinref part="X1" gate="-23" pin="S"/>
<wire x1="93.98" y1="66.04" x2="96.52" y2="66.04" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$14" class="0">
<segment>
<pinref part="13" gate="-1" pin="S"/>
<pinref part="X1" gate="-25" pin="S"/>
<wire x1="63.5" y1="15.24" x2="96.52" y2="15.24" width="0.1524" layer="91"/>
<wire x1="96.52" y1="15.24" x2="96.52" y2="63.5" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$15" class="0">
<segment>
<pinref part="2" gate="-2" pin="S"/>
<wire x1="63.5" y1="132.08" x2="114.3" y2="132.08" width="0.1524" layer="91"/>
<wire x1="114.3" y1="132.08" x2="114.3" y2="91.44" width="0.1524" layer="91"/>
<pinref part="X1" gate="-4" pin="S"/>
<wire x1="114.3" y1="91.44" x2="116.84" y2="91.44" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$16" class="0">
<segment>
<pinref part="3" gate="-2" pin="S"/>
<wire x1="63.5" y1="121.92" x2="111.76" y2="121.92" width="0.1524" layer="91"/>
<wire x1="111.76" y1="121.92" x2="111.76" y2="88.9" width="0.1524" layer="91"/>
<pinref part="X1" gate="-6" pin="S"/>
<wire x1="111.76" y1="88.9" x2="116.84" y2="88.9" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$17" class="0">
<segment>
<pinref part="4" gate="-2" pin="S"/>
<wire x1="63.5" y1="111.76" x2="109.22" y2="111.76" width="0.1524" layer="91"/>
<wire x1="109.22" y1="111.76" x2="109.22" y2="86.36" width="0.1524" layer="91"/>
<pinref part="X1" gate="-8" pin="S"/>
<wire x1="109.22" y1="86.36" x2="116.84" y2="86.36" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$18" class="0">
<segment>
<pinref part="5" gate="-2" pin="S"/>
<wire x1="63.5" y1="101.6" x2="106.68" y2="101.6" width="0.1524" layer="91"/>
<wire x1="106.68" y1="101.6" x2="106.68" y2="83.82" width="0.1524" layer="91"/>
<pinref part="X1" gate="-10" pin="S"/>
<wire x1="106.68" y1="83.82" x2="116.84" y2="83.82" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$19" class="0">
<segment>
<pinref part="6" gate="-2" pin="S"/>
<wire x1="63.5" y1="91.44" x2="73.66" y2="91.44" width="0.1524" layer="91"/>
<wire x1="73.66" y1="91.44" x2="73.66" y2="99.06" width="0.1524" layer="91"/>
<wire x1="73.66" y1="99.06" x2="104.14" y2="99.06" width="0.1524" layer="91"/>
<wire x1="104.14" y1="99.06" x2="104.14" y2="81.28" width="0.1524" layer="91"/>
<pinref part="X1" gate="-12" pin="S"/>
<wire x1="104.14" y1="81.28" x2="116.84" y2="81.28" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$20" class="0">
<segment>
<pinref part="7" gate="-2" pin="S"/>
<wire x1="63.5" y1="81.28" x2="73.66" y2="81.28" width="0.1524" layer="91"/>
<wire x1="73.66" y1="81.28" x2="73.66" y2="58.42" width="0.1524" layer="91"/>
<wire x1="73.66" y1="58.42" x2="104.14" y2="58.42" width="0.1524" layer="91"/>
<wire x1="104.14" y1="58.42" x2="104.14" y2="78.74" width="0.1524" layer="91"/>
<pinref part="X1" gate="-14" pin="S"/>
<wire x1="104.14" y1="78.74" x2="116.84" y2="78.74" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$21" class="0">
<segment>
<pinref part="8" gate="-2" pin="S"/>
<wire x1="63.5" y1="71.12" x2="71.12" y2="71.12" width="0.1524" layer="91"/>
<wire x1="71.12" y1="71.12" x2="71.12" y2="53.34" width="0.1524" layer="91"/>
<wire x1="71.12" y1="53.34" x2="106.68" y2="53.34" width="0.1524" layer="91"/>
<wire x1="106.68" y1="53.34" x2="106.68" y2="76.2" width="0.1524" layer="91"/>
<pinref part="X1" gate="-16" pin="S"/>
<wire x1="106.68" y1="76.2" x2="116.84" y2="76.2" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$22" class="0">
<segment>
<pinref part="9" gate="-2" pin="S"/>
<wire x1="63.5" y1="60.96" x2="109.22" y2="60.96" width="0.1524" layer="91"/>
<wire x1="109.22" y1="60.96" x2="109.22" y2="73.66" width="0.1524" layer="91"/>
<pinref part="X1" gate="-18" pin="S"/>
<wire x1="109.22" y1="73.66" x2="116.84" y2="73.66" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$23" class="0">
<segment>
<pinref part="10" gate="-2" pin="S"/>
<wire x1="63.5" y1="50.8" x2="111.76" y2="50.8" width="0.1524" layer="91"/>
<wire x1="111.76" y1="50.8" x2="111.76" y2="71.12" width="0.1524" layer="91"/>
<pinref part="X1" gate="-20" pin="S"/>
<wire x1="111.76" y1="71.12" x2="116.84" y2="71.12" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$24" class="0">
<segment>
<pinref part="11" gate="-2" pin="S"/>
<wire x1="63.5" y1="40.64" x2="114.3" y2="40.64" width="0.1524" layer="91"/>
<wire x1="114.3" y1="40.64" x2="114.3" y2="68.58" width="0.1524" layer="91"/>
<pinref part="X1" gate="-22" pin="S"/>
<wire x1="114.3" y1="68.58" x2="116.84" y2="68.58" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$25" class="0">
<segment>
<pinref part="12" gate="-2" pin="S"/>
<wire x1="63.5" y1="30.48" x2="116.078" y2="30.48" width="0.1524" layer="91"/>
<wire x1="116.078" y1="30.48" x2="116.078" y2="66.04" width="0.1524" layer="91"/>
<pinref part="X1" gate="-24" pin="S"/>
<wire x1="116.078" y1="66.04" x2="116.84" y2="66.04" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$26" class="0">
<segment>
<pinref part="13" gate="-2" pin="S"/>
<pinref part="X1" gate="-26" pin="S"/>
<wire x1="63.5" y1="20.32" x2="116.84" y2="20.32" width="0.1524" layer="91"/>
<wire x1="116.84" y1="20.32" x2="116.84" y2="63.5" width="0.1524" layer="91"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
