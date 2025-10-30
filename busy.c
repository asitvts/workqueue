#include <linux/module.h>
#include <linux/init.h>
#include <linux/workqueue.h>
#include <linux/kernel.h>
#include <linux/delay.h>



static struct workqueue_struct* my_workqueue;
static struct work_struct my_work;


static void work_func(struct work_struct* work){
	
	pr_info("making sure core 2 stays busy\n");
	mdelay(15000);
	pr_info("work queue is performing work, on cpu : %d on thread %s\n", smp_processor_id(), current->comm);
	return;

}

static int __init my_init(void){
	
	pr_info("-------------\n");
	pr_info("module loaded\n");
	
	my_workqueue = create_workqueue("my_work_queue");
	if(!my_workqueue){
		pr_info("error creating the work queue\n");
		return -ENOMEM;
	}
	
	
	INIT_WORK(&my_work, work_func);
	
	int ret;
	
	pr_info("queueing work on core 2\n");
	ret=queue_work_on(2,my_workqueue, &my_work);
	if(!ret){
		pr_info("this work is already queued\n");
	}
	

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












