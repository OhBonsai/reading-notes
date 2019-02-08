.PHONY: help


push:  ## 上传最新更新
	git add .
	git commit -m "auto update"
	git push
