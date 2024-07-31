from SCons.Script import DefaultEnvironment

env = DefaultEnvironment()

def before_build(source, target, env):
    print("Building and uploading LittleFS file system...")
    env.Execute("pio run --target uploadfs")

env.AddPreAction("build", before_build)
