#include <linux/module.h>
#include <linux/init.h>
#include <linux/workqueue.h>
#include <linux/kernel.h>
#include <linux/delay.h>



static struct workqueue_struct* my_workqueue;
static struct work_struct my_work;


static void work_func(struct work_struct* work){
	
	//mdelay(2000);
	pr_info("work queue is performing work, on cpu : %d on thread %s\n", smp_processor_id(), current->comm);
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
	
	int ret;
	
	pr_info("queue_work on processor : %d\n", smp_processor_id());
	ret=queue_work_on(0,my_workqueue, &my_work);
	if(!ret){
		pr_info("this work is already queued, 1\n");
	}
	
	mdelay(6000);
	
	ret=queue_work_on(3,my_workqueue, &my_work);
	if(!ret){
		pr_info("this work is already queued, 2\n");
	}
	
	
	// both queue_work and queue_work_on return false if the work being queued is already queued
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












