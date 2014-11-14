#!/usr/bin/python

import sys, getopt
from datetime import date

def main(argv):

    try:
        opts, args = getopt.getopt(argv, "mMt:h",
                            ["minor", "major", "tag=", "help"])
    except getopt.GetoptError:
        print("bumpversion -h for help.")
        sys.exit(2)

    bumpMajor = False
    bumpMinor = False
    setTag = None
    for opt, arg in opts:
        if opt in ("-h", "--help"):
            print("bumpversion increases the version number.")
            print("  -m or --minor          Increase the minor version number.")
            print("  -M or --major          Increase the major version number.")
            print("  -t or --tag <tag>      Set the tag (default: keep existing tag)")
            print("  -h or --help           This help.")
            sys.exit(0)
        elif opt in ("-m", "--minor"):
            bumpMinor = True
        elif opt in ("-M", "--major"):
            bumpMajor = True
        elif opt in ("-t", "--tag"):
            setTag = arg

    versionFile = open("src/argentum/version.h", "r")
    lines = versionFile.read().split("\n")
    versionFile.close()

    for i in range(len(lines)):
        line = lines[i]
        if line.find("version_string") == -1:
            continue
        if line.find('"') == -1:
            print("Error: malformed version_string, no quotes on line {}.".format(i))
            continue
        value = line[line.find('"')+1:]
        if value.find('"') == -1:
            print("Error: malformed version_string, no closing quote on line {}.".format(i))
            continue
        value = value[:value.find('"')]

        build = None
        if value.find("+") != -1:
            build = value[value.find("+")+1:]
            value = value[:value.find("+")]

        if value.find(".") == -1:
            print("Error: malformed version_string, no dots on line {}.".format(i))
            continue
        major = value[:value.find(".")]
        value = value[value.find(".")+1:]

        if value.find(".") == -1:
            print("Error: malformed version_string, only one dot on line {}.".format(i))
            continue
        minor = value[:value.find(".")]
        patch = value[value.find(".")+1:]

        if patch.find(".") != -1:
            print("Error: malformed version_string, too many dots on line {}.".format(i))
            continue

        tag = None
        if patch.find("-") != -1:
            tag = patch[patch.find("-")+1:]
            patch = patch[:patch.find("-")]

        try:
            major = int(major)
            minor = int(minor)
            patch = int(patch)
        except ValueError:
            print("Error: malformed version_string, major/minor/patch are not integers on line {}.".format(i))
            continue

        if setTag == None:
            if bumpMajor:
                major = major + 1
                minor = 0
                patch = 0
            elif bumpMinor:
                minor = minor + 1
                patch = 0
            else:
                patch = patch + 1

        newVersion = "{}.{}.{}".format(major, minor, patch)
        if setTag != None:
            tag = setTag
        if tag != None and tag != "":
            newVersion = newVersion + "-" + tag

        now = date.today()
        build = "{:04}{:02}{:02}".format(now.year, now.month, now.day)

        newVersion = newVersion + "+" + build
        print(newVersion)

        prefix = line[:line.find('"')]
        postfix = line[line.rfind('"')+1:]
        lines[i] = prefix + '"' + newVersion + '"' + postfix

    versionFile = open("src/argentum/version.h", "w")
    versionFile.write("\n".join(lines))
    versionFile.close()

if __name__ == "__main__":
    main(sys.argv[1:])
