import sys
from pathlib import Path

# POOR MANS' GENERIC GENERATION THINGY
# "Because macros are sometimes kinda sad" -MrPowerGamerBR

print(sys.argv)

templatesFolder = Path(sys.argv[1])
generatedDirectory = Path(sys.argv[2])

# Don't forget to update the CMakeLists to include the new files!
arrayListVariants = [
    {
        "file": "arraylist_uint32",
        "name": "Uint32ArrayList",
        "type": "uint32_t",
        "includes": [],
        "numeric": True
    },
    {
        "file": "arraylist_int32",
        "name": "Int32ArrayList",
        "type": "int32_t",
        "includes": [],
        "numeric": True
    },
    {
        "file": "arraylist_builtinfunction",
        "name": "BuiltinFunctionArrayList",
        "type": "BuiltinFunction",
        "includes": ["\"vm/builtinfunction.h\""],
        "numeric": False
    },
    {
        "file": "arraylist_builtinvariable",
        "name": "BuiltinVariableArrayList",
        "type": "BuiltinVariable",
        "includes": ["\"vm/builtinvariable.h\""],
        "numeric": False
    },
    {
        "file": "arraylist_variable",
        "name": "VariableArrayList",
        "type": "Variable",
        "includes": ["\"wad/variable.h\""],
        "numeric": False
    },
    {
        "file": "arraylist_script",
        "name": "ScriptArrayList",
        "type": "Script",
        "includes": ["\"wad/script.h\""],
        "numeric": False
    },
    {
        "file": "arraylist_function",
        "name": "FunctionArrayList",
        "type": "Function",
        "includes": ["\"wad/function.h\""],
        "numeric": False
    },
    {
        "file": "arraylist_codeentry",
        "name": "CodeEntryArrayList",
        "type": "CodeEntry",
        "includes": ["\"wad/codeentry.h\""],
        "numeric": False
    },
    {
        "file": "arraylist_rvalue",
        "name": "RValueArrayList",
        "type": "RValue",
        "includes": ["\"vm/rvalue.h\""],
        "numeric": False
    },
    {
        "file": "arraylist_callframe",
        "name": "CallFrameArrayList",
        "type": "CallFrame",
        "includes": ["\"vm/callframe.h\""],
        "numeric": False
    },
    {
        "file": "arraylist_string",
        "name": "StringArrayList",
        "type": "StarfaitString",
        "includes": ["\"starfaitstring.h\""],
        "numeric": False
    },
    {
        "file": "arraylist_pathpoint",
        "name": "PathPointArrayList",
        "type": "PathPoint",
        "includes": ["\"wad/pathpoint.h\""],
        "numeric": False
    },
    {
        "file": "arraylist_path",
        "name": "PathArrayList",
        "type": "Path",
        "includes": ["\"wad/path.h\""],
        "numeric": False
    }
]

hashMapVariants = [
    {
        "file": "hashmap_int32_rvalue",
        "name": "Int32RValueHashMap",
        "type": "RValue",
        "includes": ["\"vm/rvalue.h\""]
    }
]

with open(templatesFolder / "arraylist_numeric.h", "r", encoding="utf-8") as f:
    numericArrayListFunctions = f.read()

with open(templatesFolder / "arraylist.h", "r", encoding="utf-8") as f:
    contents = f.read()

    for arrayListVariant in arrayListVariants:
        includes = ""
        for include in arrayListVariant["includes"]:
            includes = includes + "#include " + include + "\n"
        additionalFunctions = ""
        if arrayListVariant["numeric"] is True:
            additionalFunctions = additionalFunctions + numericArrayListFunctions

        with open(generatedDirectory / (arrayListVariant["file"] + ".h"), "w") as f:
            f.write(
                ("// BEEP BOOP THIS WAS AUTOMATICALLY GENERATED - DO NOT EDIT MANUALLY!!\n" + contents)
                    .replace("__ADDITIONAL_FUNCTIONS__", additionalFunctions)
                    .replace("__ARRAY_LIST_NAME__", arrayListVariant["name"])
                    .replace("__ARRAY_LIST_TYPE__", arrayListVariant["type"])
                    .replace("__INCLUDES__", includes)
                )

with open(templatesFolder / "hashmap.h", "r", encoding="utf-8") as f:
    contents = f.read()

    for hashMapVariant in hashMapVariants:
        includes = ""
        for include in hashMapVariant["includes"]:
            includes = includes + "#include " + include + "\n"

        with open(generatedDirectory / (hashMapVariant["file"] + ".h"), "w") as f:
            f.write(
                ("// BEEP BOOP THIS WAS AUTOMATICALLY GENERATED - DO NOT EDIT MANUALLY!!\n" + contents)
                .replace("__HASH_MAP_NAME__", hashMapVariant["name"])
                .replace("__HASH_MAP_ENTRY_TYPE__", hashMapVariant["type"])
                .replace("__INCLUDES__", includes)
            )