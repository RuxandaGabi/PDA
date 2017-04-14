import java.util.Random;

public class MatrixTest {

	static int[][] mat = new int[4][4];
	static int[][] mat2 = new int[4][4];
	static int[][] result = new int[4][4];

	public static void main(String[] args) {

		Random rand = new Random();

		for (int i = 0; i < mat.length; i++) {
			for (int j = 0; j < mat[i].length; j++) {
				mat[i][j] = rand.nextInt(10);
			}
		}

		for (int i = 0; i < mat2.length; i++) {
			for (int j = 0; j < mat2[i].length; j++) {
				mat2[i][j] = rand.nextInt(10);
			}
		}

		try {

			Multiply multiply = new Multiply(4, 4);

			MatrixMultiplier thread1 = new MatrixMultiplier(multiply);
			MatrixMultiplier thread2 = new MatrixMultiplier(multiply);
			MatrixMultiplier thread3 = new MatrixMultiplier(multiply);
			MatrixMultiplier thread4 = new MatrixMultiplier(multiply);

			Thread th1 = new Thread(thread1);
			Thread th2 = new Thread(thread2);
			Thread th3 = new Thread(thread3);
			Thread th4 = new Thread(thread4);

			th1.start();
			th2.start();
			th3.start();
			th4.start();

			th1.join();
			th2.join();
			th3.join();
			th4.join();

		} catch (Exception e) {
			e.printStackTrace();
		}

		System.out.println("\n\nResult:");
		for (int i = 0; i < result.length; i++) {
			for (int j = 0; j < result[i].length; j++) {
				System.out.print(result[i][j] + " ");
			}
			System.out.println();
		}
	}

}

class Multiply extends MatrixTest {

	private int i;
	private int j;
	private int chance;

	public Multiply(int i, int j) {
		this.i = i;
		this.j = j;
		chance = 0;
	}

	public synchronized void multiplyMatrix() {

		int sum = 0;
		int a = 0;
		for (a = 0; a < i; a++) {
			sum = 0;
			for (int b = 0; b < j; b++) {
				sum = sum + mat[chance][b] * mat2[b][a];
			}
			result[chance][a] = sum;
		}

		if (chance >= i)
			return;
		chance++;
	}
}

class MatrixMultiplier implements Runnable {

	private final Multiply mul;

	public MatrixMultiplier(Multiply mul) {
		this.mul = mul;
	}

	@Override
	public void run() {
		mul.multiplyMatrix();
	}
}