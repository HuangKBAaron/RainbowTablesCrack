from subprocess import *



def exec_cmd(cmd, shell=False):
    if shell:
        response = Popen(cmd, stdout=PIPE, stderr=PIPE, shell=True)
    else:
        cmd_list = cmd.split()
        response = Popen(cmd_list, stdout=PIPE, stderr=PIPE)

    out, err = response.communicate()
    return (out, err)
