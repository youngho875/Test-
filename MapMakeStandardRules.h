#pragma once
#include <string>
#include <vector>
#include <regex>
#include <stdexcept>
namespace mm {
inline std::string RuleTrim(const std::string& s){auto a=s.find_first_not_of(" \r\n\t");return a==s.npos?"":s.substr(a,s.find_last_not_of(" \r\n\t")-a+1);}
inline std::vector<std::string> RuleSplit(const std::string& s,const std::string& delimiter){std::vector<std::string> out;size_t a=0,b;do{b=s.find(delimiter,a);out.push_back(RuleTrim(s.substr(a,b==s.npos?b:b-a)));a=b+delimiter.size();}while(b!=s.npos);return out;}
inline std::string RuleEscape(const std::string& s){std::string o;for(char c:s){if(c=='&')o+="&amp;";else if(c=='<')o+="&lt;";else if(c=='>')o+="&gt;";else if(c=='\"')o+="&quot;";else o+=c;}return o;}
inline std::string RuleJoin(const std::vector<std::string>& nodes,const char* op){if(nodes.empty())return "";if(nodes.size()==1)return nodes[0];std::string s="<ogc:"+std::string(op)+">";for(auto& n:nodes)s+=n;return s+"</ogc:"+op+">";}
// OR between VVTStyle rows; AND between attributes. Equal-value lists are OR;
// exclusion lists and numeric bounds are AND. No expression is executed as code.
inline std::string StandardFilter(const std::string& expression){
 if(RuleTrim(expression).empty())return "";
 std::vector<std::string> alternatives;
 for(auto& group:RuleSplit(expression,"||")){
  if(group.empty())throw std::runtime_error("Empty condition group");std::vector<std::string> attributes;
  for(auto& clause:RuleSplit(group,";")){
   auto colon=clause.find(':');if(colon==clause.npos)throw std::runtime_error("Condition must be FIELD:values");
   auto field=RuleTrim(clause.substr(0,colon)),value=RuleTrim(clause.substr(colon+1));
   if(field.empty()||value.empty()||field.find_first_of("<>=&|,:")!=field.npos)throw std::runtime_error("Invalid condition field/value");
   std::smatch chain;
   static const std::regex between(R"(^(-?[0-9]+(?:\.[0-9]+)?)(=<|<=|<)([A-Za-z_][A-Za-z0-9_]*)(<=|<)(-?[0-9]+(?:\.[0-9]+)?)$)");
   if(std::regex_match(value,chain,between)){
    auto lower=[](std::string t){for(auto& c:t)if(c>='A'&&c<='Z')c+=32;return t;};
    if(lower(field)!=lower(chain[3].str()))throw std::runtime_error("Range references a different field");
    value=std::string(chain[2].str()=="<"?">":">=")+chain[1].str()+","+chain[4].str()+chain[5].str();
   }
   std::vector<std::string> equal,limits;std::string inherited;
   for(auto token:RuleSplit(value,",")){
    std::string op="=";for(auto candidate:{"<>","!=",">=","<=",">","<","="})if(token.rfind(candidate,0)==0){op=candidate;token=RuleTrim(token.substr(op.size()));break;}
    if((op=="<>"||op=="!=")&&equal.empty()&&limits.empty())inherited="<>";
    if(op=="="&&!inherited.empty())op=inherited;
    if(token.empty()||token.find_first_of("<>=!;")!=token.npos)throw std::runtime_error("Unsupported condition expression: "+value);
    std::string tag=op=="="?"PropertyIsEqualTo":(op=="<>"||op=="!=")?"PropertyIsNotEqualTo":op==">="?"PropertyIsGreaterThanOrEqualTo":op=="<="?"PropertyIsLessThanOrEqualTo":op==">"?"PropertyIsGreaterThan":"PropertyIsLessThan";
    std::string node="<ogc:"+tag+"><ogc:PropertyName>"+RuleEscape(field)+"</ogc:PropertyName><ogc:Literal>"+RuleEscape(token)+"</ogc:Literal></ogc:"+tag+">";
    (op=="="?equal:limits).push_back(node);
   }
   if(!equal.empty())limits.push_back(RuleJoin(equal,"Or"));attributes.push_back(RuleJoin(limits,"And"));
  }
  alternatives.push_back(RuleJoin(attributes,"And"));
 }
 return "<ogc:Filter>"+RuleJoin(alternatives,"Or")+"</ogc:Filter>";
}
}
