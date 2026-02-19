#include<iostream>
#include<regex>
#include<string>

bool isLatex(std::string toJudge);
void handler(std::string toJudge);
std::string math_handler(std::string toJudge);
std::string replaceAllSymbols(std::string text);


int main(int argc, char * argv[]) {
	std::string exp;

	if (argc >= 2 && isLatex(argv[1])) exp = argv[1];
	else {
		std::cout << "Input LaTex expression as an argument!\n";
		return 0;
	}

	handler(exp);
}

void handler(std::string toJudge) {
	std::regex findDollar(R"(\$\$?)");
	std::string result = std::regex_replace(toJudge, findDollar, "");//remove "$" or "$$"
	result = replaceAllSymbols(result);
	std::cout << result << "\n";
}

std::string replaceAllSymbols(std::string text) {
	std::string result;
	size_t i = 0;
	while (i < text.length()) {
		if (text[i] == '\\') {
			// 按长度从长到短尝试匹配
			bool matched = false;
			for (size_t len = 15; len >= 1; len--) {
				if (i + len <= text.length()) {
					std::string candidate = text.substr(i, len);
					std::string unicode = math_handler(candidate);
					if (unicode != candidate) {
						result += unicode;
						i += len;
						matched = true;
						break;
					}
				}
			}
			if (!matched) {
				result += text[i];
				i++;
			}
		} else {
			result += text[i];
			i++;
		}
	}
	return result;
}

std::string math_handler(std::string toJudge) {
	static const std::pair<std::string, std::string> symbolMap[] = {
		// 希腊字母
		{"\\alpha", "α"}, {"\\beta", "β"}, {"\\gamma", "γ"}, {"\\delta", "δ"},
		{"\\epsilon", "ε"}, {"\\zeta", "ζ"}, {"\\eta", "η"}, {"\\theta", "θ"},
		{"\\iota", "ι"}, {"\\kappa", "κ"}, {"\\lambda", "λ"}, {"\\mu", "μ"},
		{"\\nu", "ν"}, {"\\xi", "ξ"}, {"\\pi", "π"}, {"\\rho", "ρ"},
		{"\\sigma", "σ"}, {"\\tau", "τ"}, {"\\upsilon", "υ"}, {"\\phi", "φ"},
		{"\\chi", "χ"}, {"\\psi", "ψ"}, {"\\omega", "ω"},
		// 大写希腊字母
		{"\\Gamma", "Γ"}, {"\\Delta", "Δ"}, {"\\Theta", "Θ"}, {"\\Lambda", "Λ"},
		{"\\Xi", "Ξ"}, {"\\Pi", "Π"}, {"\\Sigma", "Σ"}, {"\\Phi", "Φ"},
		{"\\Psi", "Ψ"}, {"\\Omega", "Ω"},
		// 希腊字母变体
		{"\\varepsilon", "ε"}, {"\\vartheta", "ϑ"}, {"\\varpi", "ϖ"},
		{"\\varrho", "ϱ"}, {"\\varsigma", "ς"}, {"\\varphi", "φ"},
		// 运算符
		{"\\infty", "∞"}, {"\\pm", "±"}, {"\\mp", "∓"}, {"\\times", "×"},
		{"\\div", "÷"}, {"\\cdot", "·"}, {"\\ast", "∗"}, {"\\star", "⋆"},
		{"\\circ", "∘"}, {"\\bullet", "∙"}, {"\\oplus", "⊕"}, {"\\ominus", "⊖"},
		{"\\otimes", "⊗"}, {"\\oslash", "⊘"}, {"\\odot", "⊙"},
		{"\\bigoplus", "⨁"}, {"\\bigotimes", "⨂"}, {"\\bigodot", "⨀"},
		// 关系符号
		{"\\leq", "≤"}, {"\\geq", "≥"}, {"\\neq", "≠"}, {"\\approx", "≈"},
		{"\\equiv", "≡"}, {"\\sim", "∼"}, {"\\simeq", "≃"}, {"\\cong", "≅"},
		{"\\propto", "∝"}, {"\\prec", "≺"}, {"\\succ", "≻"}, {"\\preceq", "⪯"},
		{"\\succeq", "⪰"}, {"\\ll", "≪"}, {"\\gg", "≫"}, {"\\parallel", "∥"},
		{"\\perp", "⊥"}, {"\\asymp", "≍"}, {"\\doteq", "≐"},
		// 集合符号
		{"\\subset", "⊂"}, {"\\supset", "⊃"}, {"\\subseteq", "⊆"}, {"\\supseteq", "⊇"},
		{"\\in", "∈"}, {"\\notin", "∉"}, {"\\ni", "∋"}, {"\\cap", "∩"},
		{"\\cup", "∪"}, {"\\emptyset", "∅"}, {"\\varnothing", "∅"},
		{"\\setminus", "∖"}, {"\\smallsetminus", "∖"},
		// 逻辑符号
		{"\\forall", "∀"}, {"\\exists", "∃"}, {"\\neg", "¬"}, {"\\therefore", "∴"},
		{"\\because", "∵"}, {"\\land", "∧"}, {"\\lor", "∨"}, {"\\implies", "⟹"},
		{"\\iff", "⟺"}, {"\\top", "⊤"}, {"\\bottom", "⊥"},
		// 微积分符号
		{"\\partial", "∂"}, {"\\nabla", "∇"}, {"\\sum", "∑"}, {"\\prod", "∏"},
		{"\\int", "∫"}, {"\\iint", "∬"}, {"\\iiint", "∭"}, {"\\oint", "∮"},
		{"\\sqrt", "√"}, {"\\cuberoot", "∛"}, {"\\fourthroot", "∜"},
		// 箭头符号
		{"\\rightarrow", "→"}, {"\\leftarrow", "←"}, {"\\Rightarrow", "⇒"},
		{"\\Leftarrow", "⇐"}, {"\\leftrightarrow", "↔"}, {"\\Leftrightarrow", "⇔"},
		{"\\uparrow", "↑"}, {"\\downarrow", "↓"}, {"\\Uparrow", "⇑"},
		{"\\Downarrow", "⇓"}, {"\\updownarrow", "↕"}, {"\\Updownarrow", "⇕"},
		{"\\mapsto", "↦"}, {"\\longmapsto", "⟼"}, {"\\hookrightarrow", "↪"},
		{"\\hookleftarrow", "↙"}, {"\\nearrow", "↗"}, {"\\searrow", "↘"},
		{"\\swarrow", "↙"}, {"\\nwarrow", "↖"},
		{"\\to", "→"}, {"\\gets", "←"}, {"\\impliedby", "⟸"},
		// 其他数学符号
		{"\\degree", "°"}, {"\\prime", "′"}, {"\\doubleprime", "″"},
		{"\\dots", "…"}, {"\\ldots", "…"}, {"\\cdots", "⋯"},
		{"\\vdots", "⋮"}, {"\\ddots", "⋱"},
		{"\\angle", "∠"}, {"\\triangle", "△"}, {"\\square", "□"},
		{"\\lozenge", "◊"}, {"\\diamond", "◇"},
		{"\\hbar", "ℏ"}, {"\\aleph", "ℵ"},
		{"\\beth", "ℶ"}, {"\\gimel", "ℷ"}, {"\\daleth", "ℸ"},
		{"\\Re", "ℜ"}, {"\\Im", "ℑ"}, {"\\wp", "℘"},
		{"\\ell", "ℓ"}, {"\\mho", "℧"}, {"\\Finv", "Ⅎ"}, {"\\Game", "⅁"},
		// 分数和括号
		{"\\frac", "⁄"}, {"\\lfloor", "⌊"}, {"\\rfloor", "⌋"},
		{"\\lceil", "⌈"}, {"\\rceil", "⌉"}, {"\\langle", "⟨"},
		{"\\rangle", "⟩"}, {"\\lbrace", "{"}, {"\\rbrace", "}"},
		// 特殊字符
		{"\\ ", " "}, {"\\{", "{"}, {"\\}", "}"},
		{"\\backslash", "\\"}, {"\\vert", "|"}, {"\\Vert", "‖"},
		{"\\$", "$"}, {"\\&", "&"}, {"\\%", "%"}, {"\\#", "#"},
		{"\\_", "_"}, {"\\textasciitilde", "~"}, {"\\textasciicircum", "^"},
		// 三角函数
		{"\\sin", "sin"}, {"\\cos", "cos"}, {"\\tan", "tan"},
		{"\\cot", "cot"}, {"\\sec", "sec"}, {"\\csc", "csc"},
		{"\\arcsin", "arcsin"}, {"\\arccos", "arccos"}, {"\\arctan", "arctan"},
		{"\\arccot", "arccot"}, {"\\arcsec", "arcsec"}, {"\\arccsc", "arccsc"},
		// 双曲函数
		{"\\sinh", "sinh"}, {"\\cosh", "cosh"}, {"\\tanh", "tanh"},
		{"\\coth", "coth"}, {"\\sech", "sech"}, {"\\csch", "csch"},
		// 对数和指数函数
		{"\\ln", "ln"}, {"\\lg", "lg"}, {"\\log", "log"}, {"\\exp", "exp"},
		// 极限
		{"\\lim", "lim"}, {"\\max", "max"}, {"\\min", "min"},
	};

	//find symbol
	for (const auto& [latex, unicode] : symbolMap) {
		if (toJudge == latex) {
			return unicode;
		}
	}

	return toJudge;
}

bool isLatex(std::string toJudge) {
	std::regex isLatexExp(R"(\$\$.+?\$\$|\$.+?\$)");//find pattern "$$blahblahblah$$" or "$blahblahblah$"
	return std::regex_match(toJudge, isLatexExp);
}
