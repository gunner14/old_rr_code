var swfu;
var path1='https://pcs.baidu.com/rest/2.0/pcs/file?method=upload&path=' + encodeURI("/apps/人人网盘/");

function uploadInit(){
	if (typeof(SWFUpload) === "undefined") {
		return;
	}
	
	swfu=new SWFUpload({
		flash_url : "swfupload.swf",
		upload_url: path1,
		post_params: {},
		file_size_limit : "2 GB",
		file_types : "*.*",
		file_post_name: "file",
		file_types_description : "All Files",
		file_upload_limit : 0,
		file_queue_limit : 0,
		button_cursor:SWFUpload.CURSOR.HAND,
		debug: false,
		
		button_image_url: "swf/upload.png",
		button_width: "80",
		button_height: "29",
		button_placeholder_id: "spanButtonPlaceholder",
		button_text: "",

		file_dialog_start_handler:dialogStart,
		file_queued_handler : fileQueued,
		file_queue_error_handler : fileQueueError,
		file_dialog_complete_handler : fileDialogComplete,
		upload_start_handler : uploadStart,
		upload_progress_handler : uploadProgress,
		upload_error_handler : uploadError,
		upload_success_handler : uploadSuccess,
		upload_complete_handler : uploadComplete,
		queue_complete_handler : queueComplete
	});
}
