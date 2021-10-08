S3_BUCKET=sagemaker-s3-luk
S3_FOLDER_INPUT=download
S3_FOLDER_OUTPUT=neo
S3_MODEL_INPUT_PATH=s3://sagemaker-s3-luk/download/faster_rcnn_resnet50_coco_2018_01_28.tar.gz
S3_MODEL_INPUT_PATH=s3://sagemaker-s3-luk/download/tf_frcnn.tar.gz
S3_COMPILATION_OUTPUT_PATH=s3://sagemaker-s3-luk/neo/

preparemodel:
	# [ ! -f ./faster_rcnn_resnet50_coco_2018_01_28.tar.gz ] && wget http://download.tensorflow.org/models/object_detection/faster_rcnn_resnet50_coco_2018_01_28.tar.gz
	tar -xzf faster_rcnn_resnet50_coco_2018_01_28.tar.gz
	cd faster_rcnn_resnet50_coco_2018_01_28 && tar -czf ../tf_frcnn.tar.gz frozen_inference_graph.pb

uploadmodel:
	aws s3 cp tf_frcnn.tar.gz s3://${S3_BUCKET}/${S3_FOLDER_INPUT}/

compile:
	# with aws cli get sagemaker role 
	jobname=tf-ssd-neo-`date +%H%M%S` \
			s3_model_input_path=${S3_MODEL_INPUT_PATH} \
			your_sagemaker_role_arn=${SAGEMAKER_ROLE_ARN} \
			s3_compilation_output_path=${S3_COMPILATION_OUTPUT_PATH} \
		envsubst < ./config_template_tf.json > config.json
	cat config.json
	aws sagemaker create-compilation-job \
		--cli-input-json file://config.json \
		--region eu-central-1

compileold:
	# with aws cli get sagemaker role 
	jobname=tf-ssd-neo-`date +%H%M%S` \
			s3_model_input_path="s3://sagemaker-s3-luk/download/faster_rcnn_resnet50_coco_2018_01_28.tar.gz" \
			your_sagemaker_role_arn=${SAGEMAKER_ROLE_ARN} \
			your_input_folder=${S3_FOLDER_INPUT} \
			your_output_folder=${S3_FOLDER_OUTPUT} \
			your_bucket=${S3_BUCKET} \
		envsubst < ./config_template_tf_ec2.json > config.json
	cat config.json
	aws sagemaker create-compilation-job \
		--cli-input-json file://config.json \
		--region eu-central-1

joblist:
	aws sagemaker list-compilation-jobs --query 'CompilationJobSummaries[].[CompilationJobName, CreationTime, CompilationJobStatus]' --output text

jobstatus:
	aws sagemaker describe-compilation-job --compilation-job-name $(shell aws sagemaker list-compilation-jobs --query CompilationJobSummaries[-1].CompilationJobName --output text)

