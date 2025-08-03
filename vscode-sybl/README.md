# SyBL Symbol Programming Language

Extension for SyBL programming language support in Visual Studio Code.

## 📥 Download SyBL Interpreter

**Get SyBL:** [Download from GitHub Releases](https://github.com/your-username/sybl-language/releases/latest)

1. Download `sybl-windows.zip`
2. Extract to any folder
3. Double-click `install.reg`
4. Now `.syb` files run with double-click!

## ✨ Features

- Syntax highlighting for `.syb` files
- Auto-closing brackets and quotes
- Comment support (`;:`)
- Variable recognition (`$%name%`)

## 🚀 Quick Example

Create `hello.syb`:
```sybl
;: My first SyBL program
>_ "Hello, World!"

name =#= "Developer"
age =$= 25

>_ "Hello, $%name%!"
>_ "You are $%age% years old"
```

Double-click to run!

## 📖 SyBL Syntax

### Variables
```sybl
number =$= 42              ;: Integer
pi =$.$= 3.14             ;: Float
text =#= "Hello"          ;: String
flag =&= +                ;: Boolean
```

### Output & Variables
```sybl
>_ "Simple text"
>_ "Value: $%number%"      ;: Variable in text
```

### Math Operations
```sybl
result =$= a + b          ;: +, -, *, /
```

### Conditions
```sybl
?(age >= 18)?
>_ "Adult"
?end?
```

### Loops
```sybl
@(i < 5)@
>_ "Count: $%i%"
@end@
```

## 🛠️ Usage

1. Install this extension for syntax highlighting
2. Download SyBL interpreter from GitHub
3. Create `.syb` files and double-click to run
4. Use VS Code for editing with beautiful syntax highlighting

**SyBL works like Python - just double-click to run!** 🎉