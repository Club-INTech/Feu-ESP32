import secrets

# Load PIO Env.
Import("env")
# Set Build flags
env.Append(CCFLAGS=[f'-DWIFI_SSID=\\"{secrets.SSID}\\"', f'-DWIFI_PSWD=\\"{secrets.PASSWORD}\\"', f'-DOTA_PORT={secrets.OTA_PORT}', f'-DOTA_PASWD=\\"{secrets.OTA_PASSWORD}\\"'])
# Set OTA params
env["upload_port".upper()] = secrets.OTA_IP
env["upload_flags".upper()] = f'--port={secrets.OTA_PORT} --auth="{secrets.OTA_PASSWORD}"'