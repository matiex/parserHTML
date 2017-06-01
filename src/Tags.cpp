#include "Tags.h"

Tags::Tags()
{
	mapTags["a"] = false;
	mapTags["abbr"] = false;
	mapTags["acronym"] = false;
	mapTags["address"] = false;
	mapTags["applet"] = false;
	mapTags["article"] = false;
	mapTags["aside"] = false;
	mapTags["audio"] = false;
	mapTags["b"] = false;
	mapTags["bdi"] = false;
	mapTags["bdo"] = false;
	mapTags["big"] = false;
	mapTags["blockquote"] = false;
	mapTags["body"] = false;
	mapTags["br"] = true;
	mapTags["button"] = false;
	mapTags["canvas"] = false;
	mapTags["caption"] = false;
	mapTags["center"] = false;
	mapTags["cite"] = false;
	mapTags["code"] = false;
	mapTags["colgroup"] = false;
	mapTags["command"] = false;
	mapTags["datalist"] = false;
	mapTags["dd"] = false;
	mapTags["del"] = false;
	mapTags["details"] = false;
	mapTags["dfn"] = false;
	mapTags["dialog"] = false;
	mapTags["dir"] = false;
	mapTags["div"] = false;
	mapTags["dl"] = false;
	mapTags["dt"] = false;
	mapTags["em"] = false;
	mapTags["fieldset"] = false;
	mapTags["figcaption"] = false;
	mapTags["figure"] = false;
	mapTags["font"] = false;
	mapTags["footer"] = false;
	mapTags["form"] = false;
	mapTags["frame"] = false;
	mapTags["frameset"] = false;
	mapTags["head"] = false;
	mapTags["header"] = false;
	mapTags["hgroup"] = false;
	mapTags["h1"] = false;
	mapTags["h2"] = false;
	mapTags["h3"] = false;
	mapTags["h4"] = false;
	mapTags["h5"] = false;
	mapTags["h6"] = false;
	mapTags["html"] = false;
	mapTags["i"] = false;
	mapTags["iframe"] = false;
	mapTags["ins"] = false;
	mapTags["kbd"] = false;
	mapTags["label"] = false;
	mapTags["legend"] = false;
	mapTags["li"] = false;
	mapTags["map"] = false;
	mapTags["meta"] = true;
	mapTags["mark"] = false;
	mapTags["menu"] = false;
	mapTags["meter"] = false;
	mapTags["nav"] = false;
	mapTags["noframes"] = false;
	mapTags["noscript"] = false;
	mapTags["object"] = false;
	mapTags["ol"] = false;
	mapTags["optgroup"] = false;
	mapTags["option"] = false;
	mapTags["output"] = false;
	mapTags["p"] = false;
	mapTags["pre"] = false;
	mapTags["progress"] = false;
	mapTags["q"] = false;
	mapTags["rp"] = false;
	mapTags["rt"] = false;
	mapTags["ruby"] = false;
	mapTags["s"] = false;
	mapTags["samp"] = false;
	mapTags["script"] = false;
	mapTags["section"] = false;
	mapTags["select"] = false;
	mapTags["small"] = false;
	mapTags["span"] = false;
	mapTags["strike"] = false;
	mapTags["strong"] = false;
	mapTags["style"] = false;
	mapTags["sub"] = false;
	mapTags["summary"] = false;
	mapTags["sup"] = false;
	mapTags["table"] = false;
	mapTags["tbody"] = false;
	mapTags["td"] = false;
	mapTags["textarea"] = false;
	mapTags["tfoot"] = false;
	mapTags["th"] = false;
	mapTags["thead"] = false;
	mapTags["time"] = false; 
	mapTags["title"] = false;
	mapTags["tr"] = false;
	mapTags["tt"] = false;
	mapTags["u"] = false;
	mapTags["ul"] = false;
	mapTags["var"] = false;
	mapTags["video"] = false;
	mapTags["area"] = true;
	mapTags["base"] = true;
	mapTags["basefont"] = true;
	mapTags["br"] = true;
	mapTags["col"] = true;
	mapTags["embed"] = true;
	mapTags["hr"] = true;
	mapTags["img"] = true;
	mapTags["input"] = true;
	mapTags["keygen"] = true;
	mapTags["link"] = true;
	mapTags["meta"] = true;
	mapTags["param"] = true;
	mapTags["source"] = true;
	mapTags["track"] = true;
	mapTags["wbr"] = true;
	mapTags["nobr"] = false;

	prohibitedTags.push_back("address");
	prohibitedTags.push_back("b");
	prohibitedTags.push_back("br");
	prohibitedTags.push_back("code");
	prohibitedTags.push_back("del");
	prohibitedTags.push_back("em");
	prohibitedTags.push_back("hr");
	prohibitedTags.push_back("i");
	prohibitedTags.push_back("ins");
	prohibitedTags.push_back("kbd");
	prohibitedTags.push_back("link");
	prohibitedTags.push_back("mark");
	prohibitedTags.push_back("samp");
	prohibitedTags.push_back("small");
	prohibitedTags.push_back("strong");
	prohibitedTags.push_back("sub");
	prohibitedTags.push_back("sup");
	prohibitedTags.push_back("u");
	prohibitedTags.push_back("var");

	allowedAttribs.push_back("charset");
	allowedAttribs.push_back("class");
	allowedAttribs.push_back("content");
	allowedAttribs.push_back("href");
	allowedAttribs.push_back("http-equiv");
	allowedAttribs.push_back("id");
	allowedAttribs.push_back("lang");
	allowedAttribs.push_back("name");
	allowedAttribs.push_back("source");
	allowedAttribs.push_back("src");
	allowedAttribs.push_back("type");
}

Tags& Tags::getInstance()
{
	static Tags tags;
	return tags;
}

Tags::~Tags()
{
}

bool Tags::checkIfTagIsSingle(const string & tag)
{
	return mapTags[tag];
}

bool Tags::checkIfTagExists(const string& tag)
{
	return mapTags.find(tag) != mapTags.end();
}

bool Tags::isProhibited(const string& tag)
{
	for (list<string>::iterator iter = prohibitedTags.begin(); iter != prohibitedTags.end(); ++iter){
		if ((*iter) == tag){
			return true;
		}
	}
	return false;
}

bool Tags::isAllowed(const string& tag)
{
	for (list<string>::iterator iter = allowedAttribs.begin(); iter != allowedAttribs.end(); ++iter){
		if ((*iter) == tag){
			return true;
		}
	}
	return false;
}