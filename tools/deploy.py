import requests
import json
import datetime
import deployConfig

date = datetime.datetime.now()

def sendSubdomainRequest(method, subdomain, urlSuffix, data=None, files=None):
	headers = {
		'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/133.0.0.0 Safari/537.36',
		'Authorization': f'Bearer {deployConfig.token}',
		'X-GitHub-Api-Version': '2022-11-28'
	}

	return method(
		f'https://{subdomain}.github.com/repos/{deployConfig.repo}/{urlSuffix}',
		json=data,
		files=files,
		headers=headers
	).json()

def sendAPIRequest(method, urlSuffix, data=None, files=None):
	return sendSubdomainRequest(
		method,
		'api',
		urlSuffix,
		data,
		files
	)

def sendUploadRequest(urlSuffix, data=None, files=None):
	return sendSubdomainRequest(
		requests.post,
		'uploads',
		urlSuffix,
		data,
		files
	)

# Getting latest commit
print('Getting latest commit')

commit = sendAPIRequest(
	requests.get,
	'commits/main'
)

print(f'Latest commit SHA: {commit["sha"]}')

# Creating new tag
print(f'Creating new tag with name {deployConfig.tagName}')

tag = sendAPIRequest(
	requests.post,
	'git/tags',
	{
		'tag': deployConfig.tagName,
		'message': deployConfig.tagMessage,
		'object': commit["sha"],
		'type':'commit'
	}
)

print(f'Created tag SHA: {tag["sha"]}')

# Creating new ref for tag
print('Creating new ref')

ref = sendAPIRequest(
	requests.post,
	'git/refs',
	{
		'ref': f'refs/tags/{tag["tag"]}',
		'sha': tag['sha']
	}
)

print(f'Created ref: {ref["ref"]}')

# Creating new release
print('Creating new release')

release = sendAPIRequest(
	requests.post,
	'releases',
	{
		'tag_name': tag["tag"],
		'name': deployConfig.releaseName,
		'body': deployConfig.releaseBody
	}
)

print(f'Created release ID: {release["id"]}')

# Uploading release asset
print('Uploading release asset')

asset = sendUploadRequest(
	f'releases/{release["id"]}/assets?name={deployConfig.assetName}',
	None,
	{
		deployConfig.assetName: open(deployConfig.assetPath, 'rb')
	}
)

print(f'Created asset ID: {asset["id"]}')