#include <linux/module.h>
#include <linux/init.h>
#include <linux/workqueue.h>
#include <linux/kernel.h>



static struct workqueue_struct* my_workqueue;
static struct work_struct my_work;


static void work_func(struct work_struct* work){
	
	pr_info("work queue is performing work\n");
	return;

}

static int __init my_init(void){

	pr_info("module loaded\n");
	
	my_workqueue = create_workqueue("my_work_queue");
	if(!my_workqueue){
		pr_info("error creating the work queue\n");
		return -ENOMEM;
	}
	
	
	INIT_WORK(&my_work, work_func);
	
	queue_work(my_workqueue, &my_work);
	
	
	return 0;
}

static void __exit my_exit(void){
	
	if(my_workqueue){
		flush_workqueue(my_workqueue);
		destroy_workqueue(my_workqueue);
	}
	pr_info("clean up done\n");
	
}


module_init(my_init);
module_exit(my_exit);


MODULE_AUTHOR("asit");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("work queue function");












