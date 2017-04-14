public class ProducerConsumer {
	public static void main(String[] args) {
		Object o = new Object();
		Producer p1 = new Producer(o);
		Consumer c1 = new Consumer(o);
		p1.start();
		c1.start();
	}
}

class Object {
	private int contents;
	private boolean available = false;

	public synchronized int get() {
		while (available == false) {
			try {
				wait();
			} catch (InterruptedException e) {
			}
		}
		available = false;
		notifyAll();
		return contents;
	}

	public synchronized void put(int value) {
		while (available == true) {
			try {
				wait();
			} catch (InterruptedException e) {
			}
		}
		contents = value;
		available = true;
		notifyAll();
	}
}

class Consumer extends Thread {
	private Object object;

	public Consumer(Object o) {
		object = o;
	}

	public void run() {
		int value = 0;
		for (int i = 0; i < 20; i++) {
			value = object.get();
			System.out.println("Consumer got object " + value);
		}
	}
}

class Producer extends Thread {
	private Object object;

	public Producer(Object o) {
		object = o;
	}

	public void run() {
		for (int i = 0; i < 20; i++) {
			object.put(i);
			System.out.println("Producer put object " + i);
			try {
				sleep((int) (Math.random() * 100));
			} catch (InterruptedException e) {
			}
		}
	}
}